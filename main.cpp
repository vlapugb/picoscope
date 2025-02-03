#include <memory>
#include <windows.h>
#include "logger.hpp"
#include "simplelogger.hpp"
#include "picofunctions.h"
#include "parser.hpp"


extern std::ostream out(std::cout.rdbuf());
extern SimpleLogger newlogger(out, "pico", "picologs");

HANDLE semaphore_open()
{
    HANDLE sem = OpenSemaphoreA(SEMAPHORE_MODIFY_STATE, FALSE, "wait-semaphore-5d95950d-a278-4733-a041-ee9fb05ad4e4");
    if(sem == NULL)
    {
        newlogger << LogPref::Flag(ERROR) << "Cannot open semaphore: " << GetLastError() << endl;
        return NULL;
    }
    else
    {
        return sem;
    }
}

int semaphore_release(HANDLE sem)
{
    if(!ReleaseSemaphore(sem, 1, NULL))
    {
        newlogger << LogPref::Flag(ERROR) << "Cannot release semaphore: " << GetLastError() << endl;
        return 0;
    }

    return 1;
}

int
main(int argc, char* argv[])
{
    bool flag_debug = false;
    bool flag_below = false;
    const auto &data_set = parse_xml_function(argv[1]);
    const auto &times = string_to_vector(std::get<2>(data_set));

    auto points_vec = string_to_vector(std::get<0>(data_set));
    const int32_t POINTS_VALUE = *(std::max_element(points_vec.begin(), points_vec.end()));
    const uint32_t NUMBER_OF_CHANNELS = std::get<1>(data_set);
    const uint32_t SAMPLE_FREQUENCY = std::get<3>(data_set);

    Parser parse(argc, argv);
    parse.cmdOption("--debug") == true ? flag_debug = true : flag_debug = false;
    parse.cmdOption("--below") == true ? flag_debug = true : flag_debug = false;
    //const std::string& file_name = parse.setFilename("-f", POINTS_VALUE, NUMBER_OF_CHANNELS, SAMPLE_FREQUENCY);
    //Logger logger(file_name, flag_debug);


    if ((NUMBER_OF_CHANNELS) <= 0 && (NUMBER_OF_CHANNELS) > 8)
    {
        newlogger << LogPref::Flag(ERROR) << "Incorrect NUMBER OF CHANNELS" << endl;
        return EOF;
    }
    if (SAMPLE_FREQUENCY < 0)
    {
        newlogger << LogPref::Flag(ERROR) << "Incorrect SAMPLE FREQUENCY" << endl;
        return EOF;
    }

    const auto &channels = create_channel(NUMBER_OF_CHANNELS);

    const string filepath{""};

    int16_t handle{0};

    string rs;

    if(flag_debug)
        newlogger << "handle = " << handle << endl << endl;

    auto retval_open = ps4000aOpenUnit(&handle, NULL);

    newlogger << "OPEN PICO: " << retval_open;

    int16_t enabled{true};

    PS4000A_COUPLING type_AC{PS4000A_AC};
    PS4000A_COUPLING type_DC{PS4000A_DC};
    PICO_CONNECT_PROBE_RANGE test_range{PICO_X1_PROBE_5V};

    //float analogOffset1{1.65};
    float analogOffset{0};

    int16_t start{10};
    auto retval2 = ps4000aFlashLed(handle, start);
    newlogger << retval2 << endl;

    int32_t timeIntervalNanoseconds{0};
    int32_t maxSamples{0};
    uint32_t segmentIndex{0};

    //logger.logInfo("SET TRIGGER CHANNEL CONDITIONS:");

    PS4000A_CONDITION conditions[NUMBER_OF_CHANNELS];

    conditions[0].source = channels[0];
    conditions[0].condition = PS4000A_CONDITION_TRUE;

    for (unsigned int i = 1; i < NUMBER_OF_CHANNELS; i++)
    {
        conditions[i].source = channels[i];
        conditions[i].condition = PS4000A_CONDITION_FALSE;
    }
    PS4000A_CONDITIONS_INFO info{PS4000A_CLEAR};
/*
    retval2 = ps4000aSetTriggerChannelConditions(handle, conditions, NUMBER_OF_CHANNELS, info);
    logger.log_output(retval2);
*/
    newlogger << "SET CHANNEL PICO" << endl;

    retval2 =
        ps4000aSetChannel(handle, conditions[0].source, enabled, type_DC, test_range, analogOffset);

    newlogger << retval2 << endl;

    for (int32_t i = 1; i < NUMBER_OF_CHANNELS; i++)
    {
        retval2 = ps4000aSetChannel(handle, conditions[i].source, enabled, type_AC, test_range,
                                    analogOffset);
        newlogger << retval2 << endl;
    }
    /*
    logger.logInfo("SET TRIGGER CHANNEL DIRECTIONS: ");

    PS4000A_DIRECTION directions[1];
    directions[0].channel = channels[0];
    directions[0].direction = PS4000A_ABOVE;

    retval2 = ps4000aSetTriggerChannelDirections(handle, directions, 1);
    logger.log_output(retval2);
    logger.logInfo("SET TRIGGER CHANNEL PROPERTIES: ");

    PS4000A_TRIGGER_CHANNEL_PROPERTIES channel_properties[1];

    channel_properties[0].channel = channels[0];
    channel_properties[0].thresholdUpper = 16381;
    channel_properties[0].thresholdUpperHysteresis = 4095;
    channel_properties[0].thresholdMode = PS4000A_LEVEL;

    int16_t auxOutputEnable = 0;
    int32_t autoTriggerMilliseconds{10000};

    retval2 = ps4000aSetTriggerChannelProperties(handle, channel_properties, 1, auxOutputEnable,
                                                 autoTriggerMilliseconds);
    logger.log_output(retval2);

    logger.logInfo("SET TRIGER DELAY: ");

    uint32_t delay{0};
    retval2 = ps4000aSetTriggerDelay(handle, delay);
    logger.log_output(retval2);
*/
    // Simple trigger
    newlogger << "Set simple trigger" << endl;
    int16_t enable_trigger{1};
    PS4000A_CHANNEL trig_channel{PS4000A_CHANNEL_A};
    //int16_t trig_threshhold {16381};
    int16_t trig_threshhold {8381};
    PS4000A_THRESHOLD_DIRECTION th_direction;
    if(flag_below)
    {
        th_direction = PS4000A_BELOW;
    }
    else
    {
        th_direction = PS4000A_RISING;
    }
    uint32_t trig_delay {10};
    int16_t trig_autoTrigger_ms {10000};

    auto retval_simple_trig = ps4000aSetSimpleTrigger
    (
     handle,
     enable_trigger,
     trig_channel,
     trig_threshhold,
     th_direction,
     trig_delay,
     trig_autoTrigger_ms
    );

    newlogger << "Simple trigger returned" << endl;
    newlogger << retval_simple_trig << endl;

    std::vector<int16_t *> vec_buffer(NUMBER_OF_CHANNELS, nullptr);

    int32_t bufferLth{POINTS_VALUE};

    int16_t triggerEnabled{0};
    int16_t PulseWithQualiferEnabled{0};

    newlogger << ps4000aIsTriggerOrPulseWidthQualifierEnabled(handle, &triggerEnabled, &PulseWithQualiferEnabled) << endl;


    newlogger << "prepare_for_hackrf: open semaphore" << endl;
    HANDLE sem = semaphore_open();
    if(sem == NULL)
    {
        retval2 = ps4000aStop(handle);
        newlogger << retval2 << endl;
        ps4000aCloseUnit(handle);
        newlogger << "PICO CLOSED" << endl;
        free_buffers(vec_buffer);

        return -1;
    }


    for (size_t i = 0; i < times.size(); i++)
    {
        newlogger << "GET TIMEBASE" << endl;

        uint32_t timebase = timebase_choice(SAMPLE_FREQUENCY);
        newlogger << "Timebase = " << timebase << endl;

        retval2 = ps4000aGetTimebase(handle, timebase, points_vec[i], &timeIntervalNanoseconds,
                                     &maxSamples, segmentIndex);
        newlogger << retval2 << endl;

        newlogger << "RUN BLOCK" << endl;

        int32_t noOfPreTriggerSamples{0};
        int32_t noOfPostTriggerSamples{POINTS_VALUE};

        retval2 = ps4000aRunBlock(handle, noOfPreTriggerSamples, noOfPostTriggerSamples, timebase,
                                  nullptr, segmentIndex, nullptr, nullptr);

        newlogger << retval2 << endl;
        int16_t ready{0};

        newlogger << "ready_to_read: release semaphore" << endl;
        if(semaphore_release(sem) == 0)
        {
            retval2 = ps4000aStop(handle);
            newlogger << retval2 << endl;
            ps4000aCloseUnit(handle);
            newlogger << "PICO CLOSED" << endl;
            free_buffers(vec_buffer);

            return -1;
        }

        while (ready == 0)
        {

            retval2 = ps4000aIsReady(handle, &ready);
            if(flag_debug)
            {
                newlogger << retval2 << endl;
            }
        }

        newlogger << "SET DATA BUFFER: " << endl;

        for (int32_t i = 0; i < NUMBER_OF_CHANNELS; i++)
        {
            vec_buffer[i] = new int16_t[POINTS_VALUE];
        }

        PS4000A_RATIO_MODE mode{PS4000A_RATIO_MODE_NONE};

        for (int32_t i = 0; i < NUMBER_OF_CHANNELS; i++)
        {
            retval2 = ps4000aSetDataBuffer(handle, channels[i], vec_buffer[i], bufferLth,
                                           segmentIndex, mode);
        }

        newlogger << retval2 << endl;

        newlogger << "GET VALUES" << endl;
        uint32_t startIndex{0};
        uint32_t noOfSamples{static_cast<uint32_t>(POINTS_VALUE)};
        uint32_t downSampleRatio{1};
        PS4000A_RATIO_MODE downSampleRatioMode{PS4000A_RATIO_MODE_NONE};
        int16_t overflow{0};

        retval2 = ps4000aGetValues(handle, startIndex, &noOfSamples, downSampleRatio,
                                   downSampleRatioMode, segmentIndex, &overflow);
        newlogger << retval2 << endl;

        writing_data(vec_buffer, bufferLth, NUMBER_OF_CHANNELS);

        if (times[times.size() - 1]) break;
        std::this_thread::sleep_for(
            std::chrono::duration<int64_t, std::milli>(times[i + 1] - times[i]));
    }

    newlogger << "PICO STOPED" << endl;

    retval2 = ps4000aStop(handle);
    newlogger << retval2 << endl;
    ps4000aCloseUnit(handle);
    newlogger << "PICO CLOSED" << endl;

    free_buffers(vec_buffer);

    return 0;
}
