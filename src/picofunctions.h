#pragma once
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#if defined(__WIN32) || defined(__WIN64)
#include <conio.h>
#include "windows.h"
#endif
#include <cstring>
#include "libps4000a/PicoStatus.h"
#include "libps4000a/ps4000aApi.h"
#include "pugiconfig.hpp"
#include "pugixml.hpp"

#define filename_xml "Pico_param.XML"
using std::cout;
using std::endl;
using std::string;
typedef std::tuple<string, int32_t, string, int32_t> Parse_data;

uint32_t check(const string &);
bool ShowParameterText(string, pugi::xml_node);
string return_fun(uint32_t);
Parse_data parse_xml_function(const char *);
std::vector<uint32_t> string_to_vector(string);
std::vector<PS4000A_CHANNEL> create_channel(const int32_t &);
void writing_data(const std::vector<int16_t *> &, const int32_t, const int32_t);
void free_buffers(const std::vector<int16_t *> &);
uint32_t timebase_choice(const int32_t);
void set_data_buffer(const int32_t, const int32_t);
void get_values();
