#ifndef trog_guard_macros_HPP
#define trog_guard_macros_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include <pthread.h>
#include <boost/filesystem.hpp>

#include <trog/loglevel.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>
#include <trog/formatter.hpp>
#include <trog/collector.hpp>
#include <trog/writer_threaded.hpp>
#include <trog/writer_simple.hpp>

namespace Trog {
    // using Trogger = Collector<Formatter, SinkDefault, Simple::Writer>;
}

/**
 * should be called only in one file
 * */
#ifdef TROG_GLOBAL_LEVEL
    #define TROG_SET_GLOBAL_LEVEL(global_level) \
        Trog::LogLevelType TrogGlobalLevel = global_level; 
#else
    #define TROG_SET_GLOBAL_LEVEL(global_level)
#endif
///
/// Should be callled in every compilation unit that uses logging
///
#ifdef TROG_GLOBAL_LEVEL

    #define TROG_SET_FILE_LEVEL(file_level) \
        static Trog::LogLevelType TrogFileLevel = file_level; \
        extern Trog::LogLevelType TrogGlobalLevel; 

#else                                                       \

    #define TROG_SET_FILE_LEVEL(file_level)                         \
        static Trog::LogLevelType TrogFileLevel = file_level;       \
        static Trog::LogLevelType TrogGlobalLevel = file_level; \

    #endif    

#define TROG_COLLECT(level, ...) \
    Trog::Trogger::getInstance().collect(   \
        level, \
        TrogFileLevel, \
        TrogGlobalLevel, \
        "justafiller", \
        __FILE__, \
        __func__, \
        __LINE__, \
        ##__VA_ARGS__ \
    );

inline std::string ptoa(void* p)
{
    char* pbuf;
    asprintf(&pbuf, "%p", p);;
    std::string s(pbuf);
    free(pbuf);
    return s;
}

#define TROG_ERROR(...)     TROG_COLLECT(Trog::LogLevelError, ##__VA_ARGS__)
#define TROG_WARN(...)      TROG_COLLECT(Trog::LogLevelWarn, ##__VA_ARGS__)
#define TROG_INFO(...)      TROG_COLLECT(Trog::LogLevelInfo, ##__VA_ARGS__)
#define TROG_DEBUG(...)     TROG_COLLECT(Trog::LogLevelDebug, ##__VA_ARGS__)
#define TROG_VERBOSE(...)   TROG_COLLECT(Trog::LogLevelVerbose, ##__VA_ARGS__)

#define TROG_TRACE3(...)     TROG_COLLECT(Trog::LogLevelTrace3, ##__VA_ARGS__)
#define TROG_TRACE4(...)     TROG_COLLECT(Trog::LogLevelTrace4, ##__VA_ARGS__)
#define TROG_TRACE5(...)     TROG_COLLECT(Trog::LogLevelTrace5, ##__VA_ARGS__)
#define TROG_TRACE6(...)     TROG_COLLECT(Trog::LogLevelTrace6, ##__VA_ARGS__)
#define TROG_TRACE7(...)     TROG_COLLECT(Trog::LogLevelTrace7, ##__VA_ARGS__)
#define TROG_TRACE8(...)     TROG_COLLECT(Trog::LogLevelTrace8, ##__VA_ARGS__)

#define TROG_TRACE_FD(arg_fd)  TROG_COLLECT(Trog::LogLevelTrace8, "FD:", arg_fd)
#define TROG_TRACE_CTOR()  \
    do { \
        std::string tmp = ptoa((void*)this); \
        TROG_COLLECT(Trog::LogLevelCTorTrace, "this:", tmp); \
    } while(0);

#if 0

#define LOG_LEVEL_ERROR     Trog::LogLevelError
#define LOG_LEVEL_WARN      Trog::LogLevelWarn
#define LOG_LEVEL_TRACE     Trog::LogLevelTrace
#define LOG_LEVEL_TORTRACE  Trog::LogLevelCTorTrace
#define LOG_LEVEL_FDTRACE   Trog::LogLevelFDTrace
#define LOG_LEVEL_INFO      Trog::LogLevelInfo
#define LOG_LEVEL_DEBUG     Trog::LogLevelDebug
#define LOG_LEVEL_VERBOSE   Trog::LogLevelVerbose
#define LOG_LEVEL_MAX       Trog::LogLevelVerbose


/// define the macro that sets loglevel for a file
#if ! defined(Trog_ENABLED)
    #define SET_LOGLEVEL(level)
    #define Trog_SETLEVEL(level) 
#else
    #define SET_LOGLEVEL(level) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static Trog::LogLevelType rbLogLevel = level; \
    _Pragma("clang diagnostic pop")

    #define Trog_SETLEVEL(level) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static  Trog::LogLevelType rbLogLevel = level; \
    _Pragma("clang diagnostic pop")
#endif

    
#if ! defined(Trog_ENABLED)
    #define ROBMACROFormatLog(lvl, frmt, ...)
    #define ROBMACROLog(lvl, arg1, ...)
    #define RBLOGTRACE(lvl,  arg1, ...) printf("NO LOG TRACE")
    #define RBLOGTORTRACE(lvl, arg_this ) printf("NO_CTOR")
    #define RBLOGFDTRACE(lvl, arg_fd)
#else
    #define ROBMACROFormatLog(lvl, frmt, ...) \
        Trog::Trogger::activeTrogger.logWithFormat(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*format:*/     ((char*)frmt), \
            /*var args*/    ##__VA_ARGS__\
            )

    #define ROBMACROLog(lvl, arg1, ...) \
        Trog::Trogger::activeTrogger.vlog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*arg1*/        arg1, \
            /*var args*/    ##__VA_ARGS__\
        )
    #define RBLOGTRACE(lvl, arg1, ...) \
        Trog::Trogger::activeTrogger.tracelog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*arg1*/        arg1, \
            /*var args*/    ##__VA_ARGS__\
    )
    #define RBLOGTORTRACE(lvl, arg_this) \
        Trog::Trogger::activeTrogger.torTraceLog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*this*/        (void*)arg_this \
    )
    #define RBLOGFDTRACE(lvl, arg_fd) \
        Trog::Trogger::activeTrogger.fdTraceLog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*this*/        arg_fd \
    )
#endif

//
// define the printf style log macros.
// The mixed or snake case macro names are a legacy from the original project
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

#define  LOGERROR(arg1, ...)    ROBMACROLog(LOG_LEVEL_ERROR,   arg1, ##__VA_ARGS__)
#define  LOGWARN(arg1, ...)     ROBMACROLog(LOG_LEVEL_WARN,    arg1, ##__VA_ARGS__)
#define  LOGINFO(arg1, ...)     ROBMACROLog(LOG_LEVEL_INFO,    arg1, ##__VA_ARGS__)
#define  LOGDEBUG(arg1, ...)    ROBMACROLog(LOG_LEVEL_DEBUG,   arg1, ##__VA_ARGS__)
#define  LOGVERBOSE(arg1, ...)  ROBMACROLog(LOG_LEVEL_VERBOSE, arg1, ##__VA_ARGS__)

#define  LOGTRACE(arg1, ...)    RBLOGTRACE(LOG_LEVEL_TRACE, arg1, ##__VA_ARGS__)
#define  LOGTORTRACE(...)       RBLOGTORTRACE(LOG_LEVEL_TORTRACE, this)
#define  LOGFDTRACE(fd)         RBLOGFDTRACE(LOG_LEVEL_FDTRACE, fd)

#endif
#endif
