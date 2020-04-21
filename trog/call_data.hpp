#ifndef trog_guard_call_data_HPP
#define trog_guard_call_data_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <pthread.h>
#include <boost/filesystem.hpp>
#include <trog/loglevel.hpp>


namespace Trog {

/** the data collected by a log request at the site of the call*/
struct LogCallData 
{
    public:
    std::function<void()> f;
    const char*         channel;
    const LogLevelType  level;
    const char*         file_name;
    const int           line_number;
    const char*         function_name;
    const long          tid;
    const long          pid;
    std::shared_ptr<std::string> message_sptr;

    LogCallData(
    const char*         a_channel,
    const LogLevelType  a_level,
    const char*         a_file_name,
    const int           a_line_number,
    const char*         a_function_name,
    const long          a_tid,
    const long          a_pid

    );
};


using LogCallDataSPtr = std::shared_ptr<LogCallData>;


} // namespace
#endif