#ifndef trog_guard_class_HPP
#define trog_guard_class_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <pthread.h>
#include <boost/filesystem.hpp>


namespace Trog {

extern bool logger_enabled;

/**
 * At runtime outputting of logging information is filtered by a mechanism that consists of
 * both a bit mask and logging levels.
 *
 * Log calls are filtered as follows:
 *
 *  -   each log call has associated with it a value of LogLevelTypeValues, this is the calls "loglevel"
 *      and as can be seen is a combination of an 8-bit filter mask and a log level number
 *
 *  -   each file or compilation unit has a static file global called the log_threshold which is also
 *      a value of LogLevelTypeValue
 *
 *  -   if a log calls loglevel is all zeros in filter mask part of the loglevel value  filtering is done only by level
 *      and any log call with a loglevel number > the log level number of the  files/compilation units level threshold will not be printed
 *
 *  -   if some mask bits are set in a log calls loglevel and thea files log threshold number is greater than LogLevelError
 *      the log call is printed provided log_level_mask AND threshold_mask is none zero   
 */
typedef int32_t LogLevelType;
/** The righthand 8 bits are a bit mask for Trace levels*/
const int LevelBitWidth = 64;
const int TraceBits = 8;
enum LogLevelTypeValues: int64_t {
    LogLevelCTorTrace   = 0b00000001,
    LogLevelFDTrace     = 0b00000010,
    LogLevelTrace       = 0b00000100,
    LogLevelTrace2      = 0b00001000,
    LogLevelTrace3      = 0b00010000,
    LogLevelTrace4      = 0b00100000,
    /** space for future use*/
    LogLevelError       = 1 << TraceBits,
    LogLevelWarn        = 2 << TraceBits,
    LogLevelInfo        = 3 << TraceBits,
    LogLevelDebug       = 4 << TraceBits,
    LogLevelVerbose     = 5 << TraceBits,
};

/** free functions */
std::string LogLevelText(LogLevelType level);
void setEnabled(bool on_off);

void enableForLevel(LogLevelType level);


std::ostringstream& preamble(
    std::ostringstream& os,
    std::string filename,
    long pid,
    long tid,
    std::string function_name,
    long linenumber
);

bool testLevelForActive(long level, long threshold );

class Trogger
{
public:

static Trogger activeTrogger;
static LogLevelType globalThreshold;
static LogLevelType allEnabled;

Trogger(std::ostream& os = std::cerr);
    
void logWithFormat(
    LogLevelType level, LogLevelType threshold, const char* file_name, const char* func_name, int line_number, char* format, ...);

template<typename T, typename... Types>
void vlog(
    LogLevelType level, 
    LogLevelType threshold, 
    const char* file_name, 
    const char* func_name,
    int line_number, 
    const T& firstArg, 
    const Types&... args)
{
    std::ostringstream os;
    if( levelIsActive(level, threshold) ){
        std::lock_guard<std::mutex> lg(_loggerMutex);
        std::string s = LogLevelText(level);
        os << LogLevelText(level) <<"|";
        auto tmp2 = boost::filesystem::path(file_name);
        auto tmp3 = tmp2.filename();
        auto tmp4 = tmp3.stem();
        auto pid = ::getpid();
        auto tid = pthread_self();

        preamble(os, tmp3.string(), pid, tid, func_name, line_number);
        myprint(os, firstArg, args...);
        write(STDERR_FILENO, os.str().c_str(), strlen(os.str().c_str()) );
    }
}
template<typename T, typename... Types>
void tracelog(
    LogLevelType level,
    LogLevelType threshold,
    const char* file_name,
    const char* func_name,
    int line_number,
    const T& firstArg,
    const Types&... args)
{
    std::ostringstream os;
    if( levelIsActive(level, threshold) ){
        std::lock_guard<std::mutex> lg(_loggerMutex);
        // std::string s = LogLevelText(level);
        os << "TRC|";
        long pid = ::getpid();
        long tid = pthread_self();
        preamble(os, file_name, pid, tid, func_name, line_number);
        myprint(os, firstArg, args...);
        // write(STDERR_FILENO, os.str().c_str(), strlen(os.str().c_str()) );
    }
}



void torTraceLog(
    LogLevelType level,
    LogLevelType threshold,
    const char* file_name,
    const char* func_name,
    int line_number,
    void* this_arg);

void fdTraceLog(
    LogLevelType level,
    LogLevelType threshold,
    const char* file_name,
    const char* func_name,
    int line_number,
    long fd_arg);

bool levelIsActive(LogLevelType lvl, LogLevelType threshold);

public:
    std::mutex _loggerMutex;

    std::ostream& m_outStream;
    std::vector<boost::filesystem::path> active_stems;

    std::string p_className(std::string& func_name);
    bool enabled();
    void myprint(std::ostringstream& os);

    template <typename T, typename... Types>
    void myprint (std::ostringstream& os,  const T& firstArg, const Types&... args)
    {
        os << " " << firstArg ;
        myprint(os, args...);
    }
}; //class
} // namespace
#endif