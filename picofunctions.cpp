#include "picofunctions.h"
#define DEFAULT_FREQ 400000000


uint32_t
check(const string& param)
{

    uint32_t result = stod(param);
    if (result > 0)
    {
        return uint32_t(result);
    }

    else return 1;

}


bool
ShowParameterText(string param
                 ,pugi::xml_node node)
{
    cout << "Showing parameter : " << param << std::endl;

    if (node==nullptr)
    {
        cout << "ERROR no such parameter : " << param << std::endl;
        return false;
    }
    if (strlen(node.text().get())==0)
    {
        cout << "ERROR no text for parameter : " << param << std::endl;
        return false;
    }

    cout << param << " get text: " << node.text().get() << std::endl;
    cout<< param << " get as int : " << check(node.text().get())<< std::endl;

    return true;
}


string
return_fun(uint32_t value)
{
    string rv;
    switch (value)
    {
    //return values from set channel
        case PICO_OK:                                           rv  = "PICO_OK"; break;
        case PICO_USER_CALLBACK:                                rv = "PICO_USER_CALLBACK" ; break;
        case PICO_INVALID_HANDLE:                               rv = "INVALID HANDLE"; break;
        case PICO_INVALID_CHANNEL:                              rv = "PICO_INVALID_CHANNEL"; break;
        case PICO_INVALID_VOLTAGE_RANGE:                        rv = "PICO_INVALID_VOLTAGE_RANGE"; break;
        case PICO_DRIVER_FUNCTION:                              rv = "PICO_DRIVER_FUNCTION"; break;
        case PICO_INVALID_COUPLING:                             rv =  "PICO_INVALID_COUPLING"; break;
        case PICO_INVALID_ANALOGUE_OFFSET:                      rv = "PICO_INVALID_ANALOG_OFFSET"; break;
        case PICO_WARNING_PROBE_CHANNEL_OUT_OF_SYNC:            rv = "PICO_WARNING_PROBE_CHANNEL_OUT_OF_SYNC"; break;
        case PICO_PROBE_NOT_POWERED_WITH_DC_POWER_SUPPLY:       rv = "PICO_PROBE_NOT_POWERED_WITH_DC_POWER_SUPPLY"; break;
        case PICO_PROBE_POWER_DC_POWER_SUPPLY_REQUIRED:         rv = "PICO_PROBE_POWER_DC_POWER_SUPPLY_REQUIRED"; break;
        //return values from flash led
        case PICO_BUSY:                                         rv = "PICO_BUSY"; break;
        case PICO_MEMORY:                                       rv = "PICO_MEMORY"; break;
        case PICO_INTERNAL_ERROR:                               rv = "PICO_INTERNAL_ERROR"; break;
        case PICO_POWER_SUPPLY_UNDERVOLTAGE:                    rv = "PICO_POWER_SUPPLY_UNDERVOLTAGE"; break;
        case PICO_NOT_RESPONDING:                               rv = "PICO_NOT_RESPONDING"; break;
        case PICO_POWER_SUPPLY_CONNECTED:                       rv = "PICO_POWER_SUPPLY_CONNECTED"; break;
        case PICO_POWER_SUPPLY_NOT_CONNECTED:                   rv = "PICO_POWER_SUPPLY_NOT_CONNECTED"; break;
        case PICO_TIMEOUT:                                      rv = "PICO_TIMEOUT"; break;
        case PICO_RESOURCE_ERROR:                               rv = "PICO_RESOURCE_ERROR"; break;
        case PICO_DEVICE_NOT_FUNCTIONING:                       rv = "PICO_DEVICE_NOT_FUNCTIONING"; break;
        // return values from open
        case PICO_OS_NOT_SUPPORTED:                             rv = "PICO_OS_NOT_SUPPORTED" ; break;
        case PICO_OPEN_OPERATION_IN_PROGRESS:                   rv = "PICO_OPEN_OPERATION_IN_PROGRESS"; break;
        case PICO_EEPROM_CORRUPT:                               rv = "PICO_EEPROM_CORRUPT"; break;
        case PICO_KERNEL_DRIVER_TOO_OLD:                        rv = "PICO_KERNEL_DRIVER_TOO_OLD"; break;
        case PICO_FW_FAIL:                                      rv = "PICO_FW_FAIL"; break;
        case PICO_MAX_UNITS_OPENED:                             rv = "PICO_MAX_UNITS_OPENED"; break;
        case PICO_NOT_FOUND:                                    rv = "PICO_NOT_FOUND"; break;
        case PICO_USB3_0_DEVICE_NON_USB3_0_PORT:                rv = "PICO_USB3_0_DEVICE_NON_USB3_0_PORT"; break;
        case PICO_MEMORY_FAIL:                                  rv = "PICO_MEMORY_FAIL"; break;
        case PICO_HARDWARE_VERSION_NOT_SUPPORTED:               rv = "PICO_HARDWARE_VERSION_NOT_SUPPORTED"; break;
        case PICO_NOT_USED:                                     rv = "PICO_NOT_USED"; break;
        case PICO_FPGA_FAIL:                                    rv = "PICO_FPGA_FAIL"; break;

        //return values from get timebase
        case PICO_TOO_MANY_SAMPLES:                             rv = "PICO_TOO_MANY_SAMPLES"; break;
        case PICO_INVALID_TIMEBASE:                             rv = "PICO_INVALID_TIMEBASE"; break;
        case PICO_INVALID_PARAMETER:                            rv = "PICO_INVALID_PARAMETER"; break;
        case PICO_SEGMENT_OUT_OF_RANGE:                         rv = "PICO_SEGMENT_OUT_OF_RANGE"; break;
        //return values from run block
        case PICO_INVALID_TRIGGER_CHANNEL:                      rv = "PICO_INVALID_TRIGGER_CHANNEL"; break;
        case PICO_INVALID_CONDITION_CHANNEL:                    rv = "PICO_INVALID_CONDITION_CHANNEL"; break;
        case PICO_CONFIG_FAIL:                                  rv = "PICO_CONFIG_FAIL"; break;
        case PICO_TRIGGER_ERROR:                                rv = "PICO_TRIGGER_ERROR"; break;
        case PICO_NOT_USED_IN_THIS_CAPTURE_MODE:                rv = "PICO_NOT_USED_IN_THIS_CAPTURE_MODE"; break;
        case PICO_TRIGGER_WITHIN_PRE_NOT_ALLOWED_WITH_DELAY:    rv = "PICO_TRIGGER_WITHIN_PRE_NOT_ALLOWED_WITH_DELAY"; break;
        case PICO_INVALID_NUMBER_CHANNELS_FOR_RESOLUTION:       rv = "PICO_INVALID_NUMBER_CHANNELS_FOR_RESOLUTION"; break;
        case PICO_NOT_ENOUGH_SEGMENTS:                          rv = "PICO_NOT_ENOUGH_SEGMENTS"; break;
        case PICO_NO_TRIGGER_ENABLED_FOR_TRIGGER_IN_PRE_TRIG:   rv = "PICO_NO_TRIGGER_ENABLED_FOR_TRIGGER_IN_PRE_TRIG"; break;
        //return values from set data buffer
        case PICO_RATIO_MODE_NOT_SUPPORTED:                     rv = "PICO_RATIO_MODE_NOT_SUPPORTED"; break;
        //return values from get values
        case PICO_NO_SAMPLES_AVAILABLE:                         rv = "PICO_NO_SAMPLES_AVAILABLE"; break;
        case PICO_DEVICE_SAMPLING:                              rv = "PICO_DEVICE_SAMPLING"; break;
        case PICO_NULL_PARAMETER:                               rv = "PICO_NULL_PARAMETER"; break;
        case PICO_DATA_NOT_AVAILABLE:                           rv = "PICO_DATA_NOT_AVAILABLE"; break;
        case PICO_STARTINDEX_INVALID:                           rv = "PICO_STARTINDEX_INVALID"; break;
        case PICO_INVALID_SAMPLERATIO:                          rv = "PICO_INVALID_SAMPLERATIO"; break;
        case PICO_INVALID_CALL:                                 rv = "PICO_INVALID_CALL"; break;
        case PICO_BUFFERS_NOT_SET:                              rv = "PICO_BUFFERS_NOT_SET"; break;
        case PICO_ETS_NOT_RUNNING:                              rv = "PICO_ETS_NOT_RUNNING"; break;
        //return value from SetTriggerChannelConditions
        case PICO_TOO_MANY_CHANNELS_IN_USE :                    rv = "PICO_TOO_MANY_CHANNELS_IN_USE"; break;
        case PICO_INVALID_CONDITION_INFO:                       rv = "PICO_INVALID_CONDITION_INFO"; break;
        case PICO_DUPLICATE_CONDITION_SOURCE:                   rv = "PICO_DUPLICATE_CONDITION_SOURCE"; break;
        case PICO_CANCELLED:                                    rv = "PICO_CANCELLED"; break;

        default:
                                                                rv = "unknown return value = " +  std::to_string(value);

    }
    return rv;
}


Parse_data
parse_xml_function (const char* file_name)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name);

    if (!result) 
    {
        std::cerr << "Ошибка загрузки XML-файла: " << result.description() << std::endl;
    }

    pugi::xml_node root = doc.child("root");

    string points = root.child("points").child("value").text().as_string();
    int32_t num_of_channels = root.child("num_of_channels").child("value").text().as_int();
    string times = root.child("times").child("value").text().as_string();
    int32_t sample_freq = root.child("sample_freq").child("value").text().as_int();

    return std::make_tuple(points, num_of_channels, times, sample_freq);
}


std::vector<uint32_t>
string_to_vector (string times)
{
    
    times = times.substr(1, times.length() - 2);

    std::vector<uint32_t> vec_times;
    std::stringstream ss(times);
    string token;

    while (std::getline(ss, token, ',')) 
    {
        vec_times.push_back(std::stoi(token));
    }
    
    return vec_times;
}


std::vector<PS4000A_CHANNEL>
create_channel(const int32_t& num_of_channels)
{
    std::vector<PS4000A_CHANNEL> channels;

    for (int32_t i = 0; i < num_of_channels; i++) 
    {
        switch (i) 
        {
            case 0:
               {

                PS4000A_CHANNEL trigger_channel{PS4000A_CHANNEL_A};
                channels.push_back(trigger_channel);
                break;
                }
            case 1:
                {
                    PS4000A_CHANNEL data2_channel{PS4000A_CHANNEL_B};
                channels.push_back(data2_channel);
                break;
                }
            case 2:
                {
                    PS4000A_CHANNEL data3_channel{PS4000A_CHANNEL_C};
                channels.push_back(data3_channel);
                break;
                }
            case 3:
                {
                    PS4000A_CHANNEL data4_channel{PS4000A_CHANNEL_D};
                channels.push_back(data4_channel);
                break;
                }
            case 4:
                {
                    PS4000A_CHANNEL data5_channel{PS4000A_CHANNEL_E};
                channels.push_back(data5_channel);
                break;
                }
            case 5:
                {
                    PS4000A_CHANNEL data6_channel{PS4000A_CHANNEL_F};
                channels.push_back(data6_channel);
                break;
                }
            case 6:
                {
                    PS4000A_CHANNEL data7_channel{PS4000A_CHANNEL_G};
                channels.push_back(data7_channel);
                break;
                }
            case 7:
                {
                    PS4000A_CHANNEL data8_channel{PS4000A_CHANNEL_H};
                channels.push_back(data8_channel);
                break;
                }
        }
    }
    return channels;
}


void 
writing_data(const std::vector <int16_t*>& vec_buffer
            ,const int32_t bufferLth
            ,const int32_t NUMBER_OF_CHANNELS)
{
    std::ofstream testfile;
    testfile.open("test_pico.csv", std::ios::app);
    if (testfile.is_open())
    {
        for (int i = 0; i < bufferLth; ++i) 
        {
            
            for (int j = 0; j < NUMBER_OF_CHANNELS; ++j) 
            {
                testfile << vec_buffer[j][i] << ","; // Вывод значений
            }

            testfile<<"\n";

        }
    }
    testfile.close();
}


void
free_buffers(const std::vector<int16_t*>& vec_buffer)
{
    for(const auto& it : vec_buffer)
    {
        delete[] it;
    }
}


uint32_t
timebase_choice(const int32_t SAMPLE_FREQ)
{
    return DEFAULT_FREQ / SAMPLE_FREQ - 1;
}


