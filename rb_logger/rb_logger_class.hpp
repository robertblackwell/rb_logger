#ifndef RBLOGGER_class_HPP
#define RBLOGGER_class_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <pthread.h>
#include <boost/filesystem.hpp>

// class RBLogger::Logger;    
// extern bool logger_enabled;
// extern LogLevelType globalThreshold;
// extern Logger activeLogger;
// }


namespace RBLogger {

extern bool logger_enabled;

typedef long LogLevelType;

enum LogLevelTypeValues: long {
    LogLevelTrace = 0x01,
    LogLevelCTorTrace = 0x02,
    LogLevelFDTrace = 0x04,
    LogLevelError = 1 << 3,
    LogLevelWarn =  2 << 3,
    LogLevelInfo =  3 << 3,
    LogLevelDebug = 4 << 3,
    LogLevelVerbose = 5 << 3,
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

class Logger
{
public:

static Logger activeLogger;
static LogLevelType globalThreshold;
static LogLevelType allEnabled;

Logger(std::ostream& os = std::cerr);
    
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
    boost::filesystem::path file_path = boost::filesystem::path(file_name);
    auto tmp3 = file_path.filename();
    auto tmp4 = tmp3.stem();
    if( levelIsActive(level, threshold) ){
        std::lock_guard<std::mutex> lg(_loggerMutex);
        std::string s = LogLevelText(level);
        os << "TRC|";
        auto pid = ::getpid();
        auto tid = pthread_self();

        preamble(os, tmp3.string(), pid, tid, func_name, line_number);
        myprint(os, firstArg, args...);
        write(STDERR_FILENO, os.str().c_str(), strlen(os.str().c_str()) );
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
    
private:
    std::mutex _loggerMutex;

    std::ostream& m_outStream;
    std::vector<boost::filesystem::path> active_stems;

    std::string p_className(std::string& func_name);
    bool enabled();
    bool levelIsActive(LogLevelType lvl, LogLevelType threshold);
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