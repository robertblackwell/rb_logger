#ifndef RBLOGGER_guard_macros_HPP
#define RBLOGGER_guard_macros_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <pthread.h>
#include <boost/filesystem.hpp>
#include "rb_logger_config.hpp"
#include "rb_logger_class.hpp"
    
#define LOG_LEVEL_ERROR     RBLogger::LogLevelError
#define LOG_LEVEL_WARN      RBLogger::LogLevelWarn
#define LOG_LEVEL_TRACE     RBLogger::LogLevelTrace
#define LOG_LEVEL_TORTRACE  RBLogger::LogLevelCTorTrace
#define LOG_LEVEL_FDTRACE   RBLogger::LogLevelFDTrace
#define LOG_LEVEL_INFO      RBLogger::LogLevelInfo
#define LOG_LEVEL_DEBUG     RBLogger::LogLevelDebug
#define LOG_LEVEL_VERBOSE   RBLogger::LogLevelVerbose
#define LOG_LEVEL_MAX       RBLogger::LogLevelVerbose

/// define the macro that sets loglevel for a file
#if ! defined(RBLOGGER_ENABLED)
    #define SET_LOGLEVEL(level)
    #define RBLOGGER_SETLEVEL(level) 
#else
    #define SET_LOGLEVEL(level) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static RBLogger::LogLevelType rbLogLevel = level; \
    _Pragma("clang diagnostic pop")

    #define RBLOGGER_SETLEVEL(level) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wunused-variable\"") \
    static  RBLogger::LogLevelType rbLogLevel = level; \
    _Pragma("clang diagnostic pop")
#endif

    
#if ! defined(RBLOGGER_ENABLED)
    #define ROBMACROFormatLog(lvl, frmt, ...)
    #define ROBMACROLog(lvl, arg1, ...)
    #define RBLOGTRACE(lvl,  arg1, ...) printf("NO LOG TRACE")
    #define RBLOGTORTRACE(lvl, arg_this ) printf("NO_CTOR")
    #define RBLOGFDTRACE(lvl, arg_fd)
#else
    #define ROBMACROFormatLog(lvl, frmt, ...) \
        RBLogger::Logger::activeLogger.logWithFormat(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*format:*/     ((char*)frmt), \
            /*var args*/    ##__VA_ARGS__\
            )

    #define ROBMACROLog(lvl, arg1, ...) \
        RBLogger::Logger::activeLogger.vlog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*arg1*/        arg1, \
            /*var args*/    ##__VA_ARGS__\
        )
    #define RBLOGTRACE(lvl, arg1, ...) \
        RBLogger::Logger::activeLogger.tracelog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*arg1*/        arg1, \
            /*var args*/    ##__VA_ARGS__\
    )
    #define RBLOGTORTRACE(lvl, arg_this) \
        RBLogger::Logger::activeLogger.torTraceLog(\
            /*log:*/        lvl, \
            /*threshold:*/  rbLogLevel, \
            /*file */       (char*)__FILE__, \
            /*function:*/   (char*)__PRETTY_FUNCTION__, \
            /*line:*/       __LINE__, \
            /*this*/        (void*)arg_this \
    )
    #define RBLOGFDTRACE(lvl, arg_fd) \
        RBLogger::Logger::activeLogger.fdTraceLog(\
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
