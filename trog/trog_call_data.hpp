#ifndef trog_guard_call_data_HPP
#define trog_guard_call_data_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <pthread.h>
#include <boost/filesystem.hpp>
#include <trog/trog_class.hpp>


namespace Trog {

void format_types(std::ostringstream& os);
template <typename T, typename... Types>
void format_types (std::ostringstream& os,  const T& firstArg, const Types&... args)
{
    os << " " << firstArg ;
    format_types(os, args...);
}

std::ostringstream& preamble(
    std::ostringstream& os,
    std::string filename,
    long pid,
    long tid,
    std::string function_name,
    long linenumber
);

template <typename T, typename... Types>
void fmt_log(std::ostringstream& os, char* file_name, long pid, long tid, char* function_name, int line_number, const T& firstArg, const Types&... args)
{
    preamble(os, file_name, pid, tid, function_name, line_number);
    format_types(os, firstArg, args...);
}

/** the data collected by a log request at the site of the call*/
struct LogCallData 
{
    public:

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

    ): channel(a_channel), level(a_level), file_name(a_file_name), line_number(a_line_number), function_name(a_function_name), tid(a_tid), pid(a_pid)
    {}
};


using LogCallDataSPtr = std::shared_ptr<LogCallData>;

template<typename T, typename... Types>
void collect(
    LogLevelType level,
    LogLevelType threshold,
    const char* channel,
    const char* file_name,
    const char* func_name,
    int line_number,
    const T& firstArg,
    const Types&... args)
{
    LogCallDataSPtr call_sptr = std::make_shared<LogCallData>(
        channel, level, file_name, func_name, line_number, pthread_self(), getpid()
    );
    // call_sptr->level = level;
    // call_sptr->file_name = file_name;
    // call_sptr->function_name = func_name;
    // call_sptr->line_number = line_number;
    // call_sptr->tid = pthread_self();
    // call_sptr->pid = getpid();

    std::ostringstream os;
    format_types(os, firstArg, args...);
    call_sptr->message_sptr = std::make_shared<std::string>(os.str());
}
   

} // namespace
#endif