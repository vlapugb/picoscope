#include <memory>
#include "logger.hpp"
#include "picofunctions.h"

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        return -1;
    }
    bool flag_console = false;
    if (argc > 1 && strcmp(argv[1], "--debug") == 0)
    {
        flag_console = true;
    }
    Logger logger("logger.log", flag_console);
    const auto &data_set = parse_xml_function("file.xml");
    const auto &times = string_to_vector(std::get<2>(data_set));
    auto points_vec = string_to_vector(std::get<0>(data_set));
    const int32_t POINTS_VALUE = *(std::max_element(points_vec.begin(), points_vec.end()));
    const uint32_t NUMBER_OF_CHANNELS = std::get<1>(data_set);
    const uint32_t SAMPLE_FREQUENCY = std::get<3>(data_set);

    if ((NUMBER_OF_CHANNELS) <= 0 || (NUMBER_OF_CHANNELS) > 8)
    {
        logger.logError("Incorrect NUMBER OF CHANNELS");
        return EOF;
    }
    if (SAMPLE_FREQUENCY < 0)
    {
        logger.logError("Incorrect SAMPLE FREQUENCY");
        return EOF;
    }

    const auto &channels = create_channel(NUMBER_OF_CHANNELS);
    const string filepath{""};
    int16_t handle{0};
    string rs;
    std::cout << "handle = " << handle << std::endl
              << std::endl;
    logger.logInfo("OPEN PICO:");
    auto retval_open = ps4000aOpenUnit(&handle, NULL);
    logger.log_output(retval_open);
    int16_t enabled{true};
    PS4000A_COUPLING type_AC{PS4000A_AC};
    PS4000A_COUPLING type_DC{PS4000A_DC};
    PICO_CONNECT_PROBE_RANGE test_range{PICO_X1_PROBE_5V};
    float analogOffset{0};
    int16_t start{10};
    auto retval2 = ps4000aFlashLed(handle, start);
    logger.log_output(retval2);
    int32_t timeIntervalNanoseconds{0};
    int32_t maxSamples{0};
    uint32_t segmentIndex{0};
    logger.logInfo("SET TRIGGER CHANNEL CONDITIONS:");
    PS4000A_CONDITION conditions[NUMBER_OF_CHANNELS];
    conditions[0].source = channels[0];
    conditions[0].condition = PS4000A_CONDITION_TRUE;
    for (unsigned int i = 1; i < NUMBER_OF_CHANNELS; i++)
    {
        conditions[i].source = channels[i];
        conditions[i].condition = PS4000A_CONDITION_FALSE;
    }
    PS4000A_CONDITIONS_INFO info{PS4000A_CLEAR};
    retval2 = ps4000aSetTriggerChannelConditions(handle, conditions, NUMBER_OF_CHANNELS, info);
    logger.log_output(retval2);
    logger.logInfo("SET CHANNEL PICO");
    retval2 = ps4000aSetChannel(handle, conditions[0].source, enabled, type_DC, test_range, analogOffset);
    logger.log_output(retval2);
    for (int32_t i = 1; i < NUMBER_OF_CHANNELS; i++)
    {
        retval2 = ps4000aSetChannel(handle, conditions[i].source, enabled, type_AC, test_range, analogOffset);
        logger.log_output(retval2);
    }
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
    retval2 = ps4000aSetTriggerChannelProperties(handle, channel_properties, 1, auxOutputEnable, autoTriggerMilliseconds);
    logger.log_output(retval2);
    logger.logInfo("SET TRIGER DELAY: ");
    uint32_t delay{0};
    retval2 = ps4000aSetTriggerDelay(handle, delay);
    logger.log_output(retval2);
    std::vector<int16_t *> vec_buffer(NUMBER_OF_CHANNELS, nullptr);
    int32_t bufferLth{POINTS_VALUE};
    int16_t triggerEnabled{0};
    int16_t PulseWithQualiferEnabled{0};
    for (size_t i = 0; i < times.size(); i++)
    {
        logger.logInfo("GET TIMEBASE");
        uint32_t timebase = timebase_choice(SAMPLE_FREQUENCY);
        logger.logTimebase("Timebase = ", timebase);
        retval2 = ps4000aGetTimebase(handle, timebase, points_vec[i], &timeIntervalNanoseconds, &maxSamples, segmentIndex);
        logger.log_output(retval2);
        logger.logInfo("RUN BLOCK");
        int32_t noOfPreTriggerSamples{0};
        int32_t noOfPostTriggerSamples{POINTS_VALUE};
        retval2 = ps4000aRunBlock(handle, noOfPreTriggerSamples, noOfPostTriggerSamples, timebase, nullptr, segmentIndex, nullptr, nullptr);
        logger.log_output(retval2);
        int16_t ready{0};
        while (ready == 0)
        {
            retval2 = ps4000aIsReady(handle, &ready);
            logger.log_output(retval2);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        int64_t triggerTime = 0;
        PS4000A_TIME_UNITS timeUnits = PS4000A_FS;
        retval2 = ps4000aGetTriggerTimeOffset64(handle, &triggerTime, &timeUnits, segmentIndex);
        while (return_fun(retval2) != "PICO_OK")
        {
            logger.logInfo("Trigger isn't ON");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        logger.logInfo("SET DATA BUFFER: ");
        for (int32_t i = 0; i < NUMBER_OF_CHANNELS; i++)
        {
            vec_buffer[i] = new int16_t[POINTS_VALUE];
        }
        PS4000A_RATIO_MODE mode{PS4000A_RATIO_MODE_NONE};
        for (int32_t i = 0; i < NUMBER_OF_CHANNELS; i++)
        {
            retval2 = ps4000aSetDataBuffer(handle, channels[i], vec_buffer[i], bufferLth, segmentIndex, mode);
        }
        logger.log_output(retval2);
        logger.logInfo("GET VALUES");
        uint32_t startIndex{0};
        uint32_t noOfSamples{static_cast<uint32_t>(POINTS_VALUE)};
        uint32_t downSampleRatio{1};
        PS4000A_RATIO_MODE downSampleRatioMode{PS4000A_RATIO_MODE_NONE};
        int16_t overflow{0};
        retval2 = ps4000aGetValues(handle, startIndex, &noOfSamples, downSampleRatio, downSampleRatioMode, segmentIndex, &overflow);
        logger.log_output(retval2);
        writing_data(vec_buffer, bufferLth, NUMBER_OF_CHANNELS);
        if (times[times.size() - 1])
            break;
        std::this_thread::sleep_for(std::chrono::duration<int64_t, std::milli>(times[i + 1] - times[i]));
    }
    logger.logInfo("PICO STOPED");
    retval2 = ps4000aStop(handle);
    logger.log_output(retval2);
    ps4000aCloseUnit(handle);
    logger.logInfo("PICO CLOSED");
    free_buffers(vec_buffer);
    return 0;
}
