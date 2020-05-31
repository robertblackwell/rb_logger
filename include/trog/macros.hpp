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


#ifdef TROG_DISABLE
#undef TROG_ENABLED
#else
#define TROG_ENABLED
#endif

#if !defined(TROG_FILE_LEVEL)
#error TROG FILE LEVEL not defined - this macro must be defined in every compilation unit that includes trog.hpp anywhere in its header chain
#endif


#ifdef TROG_ENABLED
    #define TROG_COLLECT(level, ...) \
        Trog::Trogger::get_instance().collect(   \
            level, \
            "justafiller", \
            __FILE__, \
            __func__, \
            __LINE__, \
            ##__VA_ARGS__ \
        );

    #define TROG_TRACE_CTOR()  \
        do { \
            std::string tmp = ptoa((void*)this); \
            TROG_COLLECT(Trog::LogLevelCTorTrace, "this:", tmp); \
        } while(0);
#else
    #define TROG_COLLECT(level, ...)
    #define TROG_TRACE_CTOR()  

#endif 

inline std::string ptoa(void* p)
{
    char* pbuf;
    asprintf(&pbuf, "%p", p);
    std::string s(pbuf);
    free(pbuf);
    return s;
}

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL >= TROG_LEVEL_ERROR)
    #define TROG_ERROR(...)  TROG_COLLECT(Trog::LogLevelError, ##__VA_ARGS__)
#else
    #define TROG_ERROR(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL >= TROG_LEVEL_WARN)
    #define TROG_WARN(...)  TROG_COLLECT(Trog::LogLevelWarn, ##__VA_ARGS__)
#else
    #define TROG_WARN(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL >= TROG_LEVEL_INFO)
    #define TROG_INFO(...) TROG_COLLECT(Trog::LogLevelInfo, ##__VA_ARGS__)
#else
    #define TROG_INFO(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL >= TROG_LEVEL_DEBUG)
    #define TROG_DEBUG(...) TROG_COLLECT(Trog::LogLevelDebug, ##__VA_ARGS__)
#else
    #define TROG_DEBUG(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL >= TROG_LEVEL_VERBOSE)
    #define TROG_VERBOSE(...) TROG_COLLECT(Trog::LogLevelVerbose, ##__VA_ARGS__)
#else
    #define TROG_VERBOSE(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_TRACE3)
    #define TROG_TRACE3(...) TROG_COLLECT(Trog::LogLevelTrace3, ##__VA_ARGS__)
#else
    #define TROG_TRACE3(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_TRACE4)
    #define TROG_TRACE4(...) TROG_COLLECT(Trog::LogLevelTrace4, ##__VA_ARGS__)
#else
    #define TROG_TRACE4(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_TRACE5)
    #define TROG_TRACE5(...) TROG_COLLECT(Trog::LogLevelTrace5, ##__VA_ARGS__)
#else
    #define TROG_TRACE5(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_TRACE6)
    #define TROG_TRACE6(...) TROG_COLLECT(Trog::LogLevelTrace6, ##__VA_ARGS__)
#else
    #define TROG_TRACE6(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_TRACE7)
    #define TROG_TRACE7(...) TROG_COLLECT(Trog::LogLevelTrace7, ##__VA_ARGS__)
#else
    #define TROG_TRACE7(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_TRACE8)
    #define TROG_TRACE8(...) TROG_COLLECT(Trog::LogLevelTrace8, ##__VA_ARGS__)
#else
    #define TROG_TRACE8(...)
#endif

#if defined(TROG_ENABLED) && (TROG_FILE_LEVEL & TROG_LEVEL_FDTRACE)
    #define TROG_TRACE_FD(arg_fd)  TROG_COLLECT(Trog::LogLevelFDTrace, "FD:", arg_fd)
#else
    #define TROG_TRACE_FD(arg_fd)
#endif


#endif
