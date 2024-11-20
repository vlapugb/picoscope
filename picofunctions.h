#pragma once

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

typedef std::tuple<string, int32_t, string, int32_t> Parse_data;


uint32_t check(const string&);

bool ShowParameterText(string, pugi::xml_node);

string return_fun(uint32_t);

Parse_data parse_xml_function(const char *);

std::vector<uint32_t> string_to_vector(string);

std::vector<PS4000A_CHANNEL> create_channel(const int32_t &);

void writing_data(const std::vector<int16_t *> &, const int32_t, const int32_t);

void free_buffers(const std::vector<int16_t *> &);

uint32_t
timebase_choice(const int32_t);

void set_data_buffer(const int32_t, const int32_t);

void get_values();
