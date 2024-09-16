//2023.11.10 1st picoscope test
//2023.11.14 add set channel function
//2023.11.17 + led blink + functions for capturing data, save buffer data to file
//2023.11.17 + level trigger
//2023.11.29 +  DataIsReady block
//2023.12.13 adopted for program for noise measurements

#include <stdio.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <fstream>
#include "pugixml.hpp"
#include <tuple>
#include <iomanip>
#include <vector>
#include <algorithm>


/* Headers for Windows */
#if defined(__WIN32) || defined(__WIN64)
#include "windows.h"
#include <conio.h>
#endif


#include <stdio.h>
#include <cstring>
#include "pugixml.hpp"
#include "libps4000a/PicoStatus.h"
#include "libps4000a/ps4000aApi.h"
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <chrono>
#include <thread>

#define filename_xml "Pico_param.XML"

using std::string;
using std::cout;
using std::endl;

uint32_t check(string param){

    uint32_t result = stod(param);
    if (result > 0)
    {
        return uint32_t(result);
    }
    else return 1;

}//check


bool ShowParameterText(string param, pugi::xml_node node)
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

    //std::cout << param << " strlen text: " << strlen(node.text().get()) << std::endl;
    cout << param << " get text: " << node.text().get() << std::endl;
    cout<< param << " get as int : " << check(node.text().get())<< std::endl;

    //std::cout << param << " get as double : " << node.text().as_double() << std::endl;
    //std::cout << param << " get as bool: " << node.text().as_bool() << std::endl;

    return true;
} //function ShowParam

string return_fun(uint32_t value)
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

} //switch
return rv;
} //func

typedef std::tuple<string, int32_t, string, int32_t> Parse_data;

Parse_data parse_xml_function (const char* file_name)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name);

    if (!result) {
        std::cerr << "Ошибка загрузки XML-файла: " << result.description() << std::endl;
    }

    pugi::xml_node root = doc.child("root");

    string points = root.child("points").child("value").text().as_string();
    int32_t num_of_channels = root.child("num_of_channels").child("value").text().as_int();
    string times = root.child("times").child("value").text().as_string();
    int32_t sample_freq = root.child("sample_freq").child("value").text().as_int();

    return std::make_tuple(points, num_of_channels, times, sample_freq);
}

std::vector<uint32_t> string_to_vector (string times)
{

    times = times.substr(1, times.length() - 2);

    std::vector<uint32_t> vec_times;
    std::stringstream ss(times);
    std::string token;

    while (std::getline(ss, token, ',')) {
        vec_times.push_back(std::stoi(token));
    }
    return vec_times;
}

std::vector<PS4000A_CHANNEL> create_channel(const int32_t& num_of_channels)
{
    std::vector<PS4000A_CHANNEL> channels;

    for (int32_t i = 0; i < num_of_channels; i++) {
        switch (i) {
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


void writing_data(const std::vector <int16_t*>& vec_buffer, const int32_t bufferLth, const int32_t NUMBER_OF_CHANNELS)
{
std::ofstream testfile;
testfile.open("test_pico.csv", std::ios::app);
    if (testfile.is_open())
    {
    for (int i = 0; i < bufferLth; ++i) {
        
        for (int j = 0; j < NUMBER_OF_CHANNELS; ++j) {
            testfile << vec_buffer[j][i] << ","; // Вывод значений
        }
        testfile<<"\n";
    }

    testfile.close();
  }
}



int main()
{

 // Загрузка XML-файла
const auto& data_set = parse_xml_function("file.xml");
const auto& times = string_to_vector(std::get<2>(data_set));

auto points_vec = string_to_vector(std::get<0>(data_set));
const int32_t POINTS_VALUE = *(std::max_element(points_vec.begin(), points_vec.end()));
const int32_t NUMBER_OF_CHANNELS = std::get<1>(data_set);
const int32_t SAMPLE_FREQUENCY = std::get<3>(data_set);


const auto& channels = create_channel(NUMBER_OF_CHANNELS);

const string filepath{""};

int16_t handle{0};
uint32_t return_value_open{0};
string rs;
cout << "\n";
cout << "handle = " << handle << "\n\n";

cout << "////////////////open pico//////////////"<< "\n\n";

auto retval_open = ps4000aOpenUnit(&handle, NULL);
rs = return_fun(retval_open);

cout << "\n" << "rs = "<< rs << "\n";

PS4000A_CHANNEL test_channel {PS4000A_CHANNEL_A} ;

int16_t enabled {true};

PS4000A_COUPLING type_AC  {PS4000A_AC};
PS4000A_COUPLING type_DC {PS4000A_DC};
PICO_CONNECT_PROBE_RANGE test_range {PICO_X1_PROBE_5V};

float analogOffset  {0};

int16_t start{10};
auto r = ps4000aFlashLed(handle, start);
rs = return_fun(r);
cout << "\n" << rs << "\n";



//ps4000aGetTimebase() – find out what timebases are available
cout << "////////////////get timebase pico//////////////"<< "\n\n";
uint32_t timebase {0};// sampling frequency = 50MHz/(timebase-2) sampling interval = 20ns*(timebase-2)

// after measurements find out timebase = 0 -> sampling interval = 13 ns; timebase = 1 -> sampling interval = 25ns

int32_t  timeIntervalNanoseconds{0};
int32_t  maxSamples{0};
uint32_t segmentIndex{0};

r = ps4000aGetTimebase( handle, timebase,  POINTS_VALUE,  &timeIntervalNanoseconds, &maxSamples, segmentIndex);
rs = return_fun(r);
cout << "\n" << "rs = "<< rs << "\n";
cout<<"time interval (ns)"<<timeIntervalNanoseconds<<"\n";



//ps4000aSetTriggerChannelConditions() – specify which channels to trigger on
cout << "////////////////ps4000aSetTriggerChannelConditions()//////////////"<< endl<< endl;

PS4000A_CONDITION conditions[1];

conditions[0].source = channels[0];
conditions[0].condition = PS4000A_CONDITION_TRUE;


PS4000A_CONDITIONS_INFO info {PS4000A_CLEAR};

r = ps4000aSetTriggerChannelConditions( handle, conditions,  NUMBER_OF_CHANNELS,  info);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;



cout << "////////////////set channel pico//////////////"<< endl<< endl;

r = ps4000aSetChannel(handle, conditions[0].source, enabled, type_DC, test_range, analogOffset);

for (size_t i = 1; i <NUMBER_OF_CHANNELS; i++)
{
    r = ps4000aSetChannel(handle, conditions[i].source, enabled, type_AC, test_range, analogOffset);
}

rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;



cout << "////////////////ps4000aSetTriggerChannelDirections()//////////////"<< endl<< endl;

PS4000A_DIRECTION directions[1];

    directions[0].channel = channels[0];
    directions[0].direction = PS4000A_ABOVE;

r = ps4000aSetTriggerChannelDirections(handle, directions, 1);

rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


//ps4000aSetTriggerChannelProperties() – set up trigger thresholds
cout << "////////////////ps4000aSetTriggerChannelProperties()//////////////"<< endl<< endl;

PS4000A_TRIGGER_CHANNEL_PROPERTIES  channel_properties [1];

    channel_properties[0].channel = channels[0];
    channel_properties[0].thresholdUpper = 16381 ; //Threshold voltage in 16 bit scale from set voltage on channel
    channel_properties[0].thresholdUpperHysteresis = 4095;
    channel_properties[0].thresholdMode = PS4000A_LEVEL;


int16_t auxOutputEnable;
int32_t autoTriggerMilliseconds {10000};

r = ps4000aSetTriggerChannelProperties(handle, channel_properties, 1, auxOutputEnable, autoTriggerMilliseconds);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


//ps4000aSetTriggerDelay() – set up post-trigger delay

cout << "////////////////ps4000aSetTriggerDelay()//////////////" << endl<< endl;

uint32_t delay {0};
r =  ps4000aSetTriggerDelay(handle, delay);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

std::vector <int16_t*> vec_buffer(NUMBER_OF_CHANNELS, nullptr);
int32_t bufferLth{POINTS_VALUE};

cout<<POINTS_VALUE;


for(size_t i=0; i < 2; i++) // open for
{

r = ps4000aGetTimebase( handle, timebase,  points_vec[i],  &timeIntervalNanoseconds, &maxSamples, segmentIndex);
rs = return_fun(r);
cout << "\n" << "rs = "<< rs << "\n";

cout<<POINTS_VALUE;

//ps4000aRunBlock() – start block mode
cout << "////////////////ps4000aRunBlock()//////////////"<< endl<< endl;
int32_t noOfPreTriggerSamples {0};
int32_t * timeIndisposedMs;
ps4000aBlockReady lpReady;
void * pParameter;

r = ps4000aRunBlock(handle, 0, static_cast<int32_t>(points_vec[i]),  timebase, nullptr, segmentIndex , nullptr, nullptr);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

cout << "////////////////ps4000isReady()//////////////"<< endl<< endl;//check if data is ready

int16_t ready{0};
while (ready==0)
{
cout << "Data NOT ready"<<endl;
r = ps4000aIsReady(handle, &ready);
}



//ps4000aSetDataBuffer() – register data buffer with driver
cout << "////////////////SetDataBuffer//////////////"<< endl<< endl;

for(size_t i = 0; i < NUMBER_OF_CHANNELS; i++)
{
    vec_buffer[i] = new int16_t[POINTS_VALUE];
}

//uint32_t segmentIndex;
PS4000A_RATIO_MODE mode {PS4000A_RATIO_MODE_NONE};

for (size_t i = 0; i <NUMBER_OF_CHANNELS; i++)
{
    r = ps4000aSetDataBuffer(handle, channels[i], vec_buffer[i], bufferLth, segmentIndex, mode);
}

rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;



cout << "////////////////GetValues//////////////"<< endl<< endl;
uint32_t startIndex{0};
uint32_t  noOfSamples {static_cast<uint32_t>(POINTS_VALUE)}; //modified from 102 400
uint32_t downSampleRatio {1};
PS4000A_RATIO_MODE downSampleRatioMode{PS4000A_RATIO_MODE_NONE};
//uint32_t segmentIndex;
int16_t  overflow{0};

r = ps4000aGetValues(handle, startIndex, &points_vec[i], downSampleRatio, downSampleRatioMode,  segmentIndex, &overflow);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;

writing_data(vec_buffer, bufferLth, NUMBER_OF_CHANNELS);

if(times[times.size()-1]) break;
std::this_thread::sleep_for(std::chrono::duration<int64_t, std::milli>(times[i+1] -times[i]));

} //for 

//ps4000aStop() – stop data capture
cout << "////////////////Pico Stop//////////////"<< endl<< endl;

r = ps4000aStop(handle);
rs = return_fun(r);
cout << endl << "rs = "<< rs << endl;


ps4000aCloseUnit(handle);
cout << "////////////////PICO CLOSED//////////////"<< endl<< endl;


cout << endl;
cout << "handle = " << handle << endl;
cout << endl;


    return 0;
} //int main end



