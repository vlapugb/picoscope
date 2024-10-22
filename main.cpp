
#include "picofunctions.h"
#include <memory>

int
main()
{

    const auto& data_set = parse_xml_function("file.xml");
    const auto& times = string_to_vector(std::get<2>(data_set));

    auto points_vec = string_to_vector(std::get<0>(data_set));
    const int32_t POINTS_VALUE = *(std::max_element(points_vec.begin(), points_vec.end()));
    const int32_t NUMBER_OF_CHANNELS = std::get<1>(data_set);
    const int32_t SAMPLE_FREQUENCY = std::get<3>(data_set);

    if((NUMBER_OF_CHANNELS) <= 0 && (NUMBER_OF_CHANNELS) > 8)
    {
        std::cerr << "Incorrect NUMBER OF CHANNELS";
        return EOF;
    }
    if(SAMPLE_FREQUENCY < 0)
    {
        std::cerr << "Incorrect SAMPLE FREQUENCY";
        return EOF;
    }
    

    const auto& channels = create_channel(NUMBER_OF_CHANNELS);

    const string filepath{""};

    int16_t handle{0};

    string rs;

    cout << "handle = " << handle << endl << endl;

    cout << "OPEN PICO:"<< endl << endl;

    auto retval_open = ps4000aOpenUnit(&handle, NULL);
    rs = return_fun(retval_open);

    cout << endl << "Pico State = "<< rs << endl;

    int16_t enabled {true};

    PS4000A_COUPLING type_AC  {PS4000A_AC};
    PS4000A_COUPLING type_DC {PS4000A_DC};
    PICO_CONNECT_PROBE_RANGE test_range {PICO_X1_PROBE_5V};

    float analogOffset  {0};

    int16_t start{10};
    auto r = ps4000aFlashLed(handle, start);
    rs = return_fun(r);
    cout << endl << rs << endl;

    int32_t  timeIntervalNanoseconds{0};
    int32_t  maxSamples{0};
    uint32_t segmentIndex{0};


    cout << "SET TRIGGER CHANNEL CONDITIONS:"<< endl << endl;

    PS4000A_CONDITION conditions[1];

    conditions[0].source = channels[0];
    conditions[0].condition = PS4000A_CONDITION_TRUE;


    PS4000A_CONDITIONS_INFO info {PS4000A_CLEAR};

    r = ps4000aSetTriggerChannelConditions( handle, conditions,  NUMBER_OF_CHANNELS, info);
    rs = return_fun(r);
    cout << endl << "PICO State = "<< rs << endl;

    cout << "SET CHANNEL PICO" << endl << endl;

    r = ps4000aSetChannel(handle, conditions[0].source, enabled, type_DC, test_range, analogOffset);

    for (int32_t i = 1; i <NUMBER_OF_CHANNELS; i++)
    {
        r = ps4000aSetChannel(handle, conditions[i].source, enabled, type_AC, test_range, analogOffset);
    }

    rs = return_fun(r);
    cout << endl << "rs = "<< rs << endl;



    cout << "SET TRIGGER CHANNEL DIRECTIONS: "<< endl<< endl;

    PS4000A_DIRECTION directions[1];
    directions[0].channel = channels[0];
    directions[0].direction = PS4000A_ABOVE;

    r = ps4000aSetTriggerChannelDirections(handle, directions, 1);

    rs = return_fun(r);
    cout << endl << "PICO State = "<< rs << endl;

    cout << "SET TRIGGER CHANNEL PROPERTIES: "<< endl<< endl;

    PS4000A_TRIGGER_CHANNEL_PROPERTIES  channel_properties [1];

    channel_properties[0].channel = channels[0];
    channel_properties[0].thresholdUpper = 16381 ;
    channel_properties[0].thresholdUpperHysteresis = 4095;
    channel_properties[0].thresholdMode = PS4000A_LEVEL;

    int16_t auxOutputEnable = 0;
    int32_t autoTriggerMilliseconds {10000};

    r = ps4000aSetTriggerChannelProperties(handle, channel_properties, 1, auxOutputEnable , autoTriggerMilliseconds);
    rs = return_fun(r);
    cout << endl << "PICO State = "<< rs << endl;


    cout << "SET TRIGER DELAY: " << endl<< endl;

    uint32_t delay {0};
    r =  ps4000aSetTriggerDelay(handle, delay);
    rs = return_fun(r);
    cout << endl << "PICO State = "<< rs << endl;

    std::vector <int16_t*> vec_buffer(NUMBER_OF_CHANNELS, nullptr);

    int32_t bufferLth{POINTS_VALUE};
    cout << "buffer length: " << bufferLth;
    cout<<POINTS_VALUE;


    for(size_t i=0; i < times.size(); i++)
    {
        cout << "GET TIMEBASE:";

        uint32_t timebase = timebase_choice(SAMPLE_FREQUENCY);
        cout << "PICO Timebase = "<< timebase << endl;

        r = ps4000aGetTimebase(handle, timebase,  points_vec[i],  &timeIntervalNanoseconds, &maxSamples, segmentIndex);
        rs = return_fun(r);
        cout << endl << "PICO State = "<< rs << endl;

        cout << "RUN BLOCK"<< endl<< endl;
        int32_t noOfPreTriggerSamples {0};
        int32_t noOfPostTriggerSamples {POINTS_VALUE};

        r = ps4000aRunBlock(handle,noOfPreTriggerSamples, noOfPostTriggerSamples,  timebase, nullptr, segmentIndex , nullptr, nullptr);

        rs = return_fun(r);
        cout << endl << "PICO State = "<< rs << endl;

        int16_t ready{0};

        while (ready==0)
        {
            cout << "IS DATA READY: " << '\t' << "NOT ready" << endl;
            ;
            r = ps4000aIsReady(handle, &ready);
        }

        cout << "SET DATA BUFFER: "<< endl<< endl;

        for(int32_t i = 0; i < NUMBER_OF_CHANNELS; i++)
        {
            vec_buffer[i] = new int16_t[POINTS_VALUE];
        }

        PS4000A_RATIO_MODE mode {PS4000A_RATIO_MODE_NONE};

        for (int32_t i = 0; i <NUMBER_OF_CHANNELS; i++)
        {
            r = ps4000aSetDataBuffer(handle, channels[i], vec_buffer[i], bufferLth, segmentIndex, mode);
        }

        rs = return_fun(r);
        cout << endl << "PICO State = "<< rs << endl;

        cout << "GET VALUES: "<< endl<< endl;
        uint32_t startIndex{0};
        uint32_t  noOfSamples {static_cast<uint32_t>(POINTS_VALUE)};
        uint32_t downSampleRatio {1};
        PS4000A_RATIO_MODE downSampleRatioMode{PS4000A_RATIO_MODE_NONE};
        int16_t  overflow{0};

        r = ps4000aGetValues(handle, startIndex, &noOfSamples, downSampleRatio, downSampleRatioMode,  segmentIndex, &overflow);
        cout << "samples: " << noOfSamples;
        rs = return_fun(r);
        cout << endl << "PICO State = "<< rs << endl;

        writing_data(vec_buffer, bufferLth, NUMBER_OF_CHANNELS);

        if(times[times.size()-1]) break;
        std::this_thread::sleep_for(std::chrono::duration<int64_t, std::milli>(times[i+1] -times[i]));

    }

    cout << "PICO STOP"<< endl<< endl;

    r = ps4000aStop(handle);
    rs = return_fun(r);
    cout << endl << "rs = "<< rs << endl;

    ps4000aCloseUnit(handle);
    cout << "PICO CLOSED"<< endl<< endl;

    free_buffers(vec_buffer);

    return 0;
}
