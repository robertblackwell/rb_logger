#ifndef RBLOGGER_class_HPP
#define RBLOGGER_class_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <pthread.h>
#include <boost/filesystem.hpp>
namespace RBLogging{
    
enum LogLevelType: long {
    LogLevelTrace = 0x01,
    LogLevelCTorTrace = 0x02,
    LogLevelFDTrace = 0x04,
    LogLevelError = 1 << 3,
    LogLevelWwarn =  2 << 3,
    LogLevelInfo =  3 << 3,
    LogLevelDebug = 4 << 3,
    LogLevelVerbose = 5 << 3,

};
    
extern bool logger_enabled;
extern LogLevelType globalThreshold;

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
    if( level == LOG_LEVEL_TRACE) {
        assert(true);
    }
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
};
#pragma mark - dclare a logger
#undef LOGGER_SINGLE
#ifdef LOGGER_SINGLE
extern Logger activeLogger;
#else
static Logger activeLogger{};
#endif

#pragma mark - macros
// want to default to "ON" - disable Log by #define RBLOGGER_OFF
//#define RBLOGGER_OFF

///
/// These trun off/on tracing of special attributes
///

//#define NO_TRACE          // trace key points in the message traffic
//#define NO_CTORTRACE      // trace constructors and destructors
//#define NO_FDTRACE       // trace file descriptors
#define RBLOGGER_ON
#if !defined(RBLOGGER_OFF) || defined(RBLOGGER_ON) || defined(RBLOGGER_ENABLED)
    #define RBLOGGER_ENABLED
#else
    #undef RBLOGGER_ENABLED
#endif
    
#if ! defined(RBLOGGER_ENABLED)
    
    #define ROBMACROFormatLog(lvl, frmt, ...)
    #define ROBMACROLog(lvl, arg1, ...)
#else
    #define ROBMACROFormatLog(lvl, frmt, ...) \
        RBLogging::activeLogger.logWithFormat(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*format:*/     ((char*)frmt), \
            /*var args*/    ##__VA_ARGS__\
            )

    #define ROBMACROLog(lvl, arg1, ...) \
        RBLogging::activeLogger.vlog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*arg1*/        arg1, \
            /*var args*/    ##__VA_ARGS__\
        )
#endif

// tracing (as compared to loggin)
#ifdef NO_TRACE
    #define RBLOGTRACE(lvl,  arg1, ...) printf("NO LOG TRACE")
#else
    #define RBLOGTRACE(lvl, arg1, ...) \
        printf("NO_TRACE about to call functions"); \
        RBLogging::activeLogger.tracelog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*arg1*/        arg1, \
            /*var args*/    ##__VA_ARGS__\
    )
#endif

// constructore/destructor trace
#ifdef NO_CTORTRACE
    #define RBLOGTORTRACE(lvl, arg_this ) printf("NO_CTOR")
#else
    #define RBLOGTORTRACE(lvl, arg_this) \
        RBLogging::activeLogger.torTraceLog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*this*/        (void*)arg_this \
    )
#endif

#ifdef NO_FDTRACE
    #define RBLOGFDTRACE(lvl, arg_fd)
#else
    #define RBLOGFDTRACE(lvl, arg_fd) \
        RBLogging::activeLogger.fdTraceLog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*this*/        arg_fd \
    )
#endif

//
// define the printf style log macros
//
#define  FLogError(frmt, ...)   ROBMACROFormatLog(LOG_LEVEL_ERROR, frmt, ##__VA_ARGS__)
#define  FLogWarn(frmt, ...)    ROBMACROFormatLog(LOG_LEVEL_WARN, frmt, ##__VA_ARGS__)
#define  FLogInfo(frmt, ...)    ROBMACROFormatLog(LOG_LEVEL_INFO, frmt, ##__VA_ARGS__)
#define  FLogDebug(frmt, ...)   ROBMACROFormatLog(LOG_LEVEL_DEBUG, frmt, ##__VA_ARGS__)
#define  FLogVerbose(frmt, ...) ROBMACROFormatLog(LOG_LEVEL_VERBOSE, frmt, ##__VA_ARGS__)

//
// define the NON printf style log macros - simply a list of things to print
//
#define  VLogError(arg1, ...)   ROBMACROLog(LOG_LEVEL_ERROR,   arg1, ##__VA_ARGS__)
#define  VLogWarn(arg1, ...)    ROBMACROLog(LOG_LEVEL_WARN,    arg1, ##__VA_ARGS__)
#define  VLogInfo(arg1, ...)    ROBMACROLog(LOG_LEVEL_INFO,    arg1, ##__VA_ARGS__)
#define  VLogDebug(arg1, ...)   ROBMACROLog(LOG_LEVEL_DEBUG,   arg1, ##__VA_ARGS__)
#define  VLogVerbose(arg1, ...) ROBMACROLog(LOG_LEVEL_VERBOSE, arg1, ##__VA_ARGS__)

#define  LogError(arg1, ...)    ROBMACROLog(LOG_LEVEL_ERROR,   arg1, ##__VA_ARGS__)
#define  LogWarn(arg1, ...)     ROBMACROLog(LOG_LEVEL_WARN,    arg1, ##__VA_ARGS__)
#define  LogInfo(arg1, ...)     ROBMACROLog(LOG_LEVEL_INFO,    arg1, ##__VA_ARGS__)
#define  LogDebug(arg1, ...)    ROBMACROLog(LOG_LEVEL_DEBUG,   arg1, ##__VA_ARGS__)
#define  LogVerbose(arg1, ...)  ROBMACROLog(LOG_LEVEL_VERBOSE, arg1, ##__VA_ARGS__)

#define  LogTrace(arg1, ...)    RBLOGTRACE(LOG_LEVEL_TRACE, arg1, ##__VA_ARGS__)
#define  LogTorTrace(...)       RBLOGTORTRACE(LOG_LEVEL_TORTRACE, this)
#define  LogFDTrace(fd)         RBLOGFDTRACE(LOG_LEVEL_FDTRACE, fd)

} // namespace RBLogging

#endif // header guard

#if ! defined(RBLOGGER_ENABLED)

    #define SET_LOGLEVEL(level)
    #define RBLOGGER_SETLEVEL(level) 

#else

    #define SET_LOGLEVEL(level) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static RBLogging::LogLevelType rbLogLevel = level; \
    _Pragma("clang diagnostic pop")

    #define RBLOGGER_SETLEVEL(level) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static  RBLogging::LogLevelType rbLogLevel = level; \
    _Pragma("clang diagnostic pop")

#endif
