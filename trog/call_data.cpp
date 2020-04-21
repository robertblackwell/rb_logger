
#include <trog/call_data.hpp>

//
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <pthread.h>
#include <boost/filesystem.hpp>

namespace Trog {

LogCallData::LogCallData(
const char*         a_channel,
const LogLevelType  a_level,
const char*         a_file_name,
const int           a_line_number,
const char*         a_function_name,
const long          a_tid,
const long          a_pid

):  channel(a_channel), 
    level(a_level), 
    file_name(a_file_name), 
    line_number(a_line_number), 
    function_name(a_function_name), 
    tid(a_tid), 
    pid(a_pid)
{
    f = nullptr;
    // std::cout << "inside LogCallData CTOR" << std::endl;
}
} //namespace
