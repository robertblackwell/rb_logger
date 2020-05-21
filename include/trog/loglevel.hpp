#ifndef trog_guard_loglevel_HPP
#define trog_guard_loglevel_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <bitset>
#include <cstdint>
#include <assert.h>
//
// This file defines that macros that represent different log levels and channels
//
namespace Trog {

typedef int64_t LogLevelType;
/**
 *  The righthand 16 bits are a bit mask for Trace levels. These are treated like "channels"
 *  in other logging libraries. They are either on or off almost regardless of the prevailing
 *  threshold level. 
 */
const int LevelBitWidth = 64;
#define TROG_BIT_WIDTH 64
const int TraceBits = 16;
#define TROG_TRACE_BITS 16
// 16 trace channels - each represented by a bit in the level value
#define TROG_LEVEL_CTOR     0b0000000000000001
#define TROG_LEVEL_FDTRACE  0b0000000000000010
#define TROG_LEVEL_TRACE3   0b0000000000000100
#define TROG_LEVEL_TRACE4   0b0000000000001000
#define TROG_LEVEL_TRACE5   0b0000000000010000
#define TROG_LEVEL_TRACE6   0b0000000000100000
#define TROG_LEVEL_TRACE7   0b0000000001000000
#define TROG_LEVEL_TRACE8   0b0000000010000000
#define TROG_TRACE_MAX      0b1000000000000000
#define TROG_TRACE_ALL      0b1111111111111111
#define TROG_TRACE_MASK     TROG_TRACE_ALL
/** space for future use*/
// log levels are consecutive values "above" the channel/trace bits
#define TROG_LEVEL_ERROR    (0x1 << (TROG_TRACE_BITS))
#define TROG_LEVEL_WARN     (0x2 << (TROG_TRACE_BITS))
#define TROG_LEVEL_INFO     (0x3 << (TROG_TRACE_BITS))
#define TROG_LEVEL_DEBUG    (0x4 << (TROG_TRACE_BITS))
#define TROG_LEVEL_VERBOSE  (0x5 << (TROG_TRACE_BITS))
#define TROG_LEVEL_ALL      TROG_LEVEL_VERBOSE

enum LogLevelTypeValues: int64_t {
    LogLevelCTorTrace   =   TROG_LEVEL_CTOR,
    LogLevelFDTrace     =   TROG_LEVEL_FDTRACE,
    LogLevelTrace3      =   TROG_LEVEL_TRACE3,
    LogLevelTrace4      =   TROG_LEVEL_TRACE4,
    LogLevelTrace5      =   TROG_LEVEL_TRACE5,
    LogLevelTrace6      =   TROG_LEVEL_TRACE6,
    LogLevelTrace7      =   TROG_LEVEL_TRACE7,
    LogLevelTrace8      =   TROG_LEVEL_TRACE8,
    LogLevelTraceAll    =   TROG_TRACE_ALL,

    /** space for future use*/

    LogLevelError       =   TROG_LEVEL_ERROR,
    LogLevelWarn        =   TROG_LEVEL_WARN,
    LogLevelInfo        =   TROG_LEVEL_INFO,
    LogLevelDebug       =   TROG_LEVEL_DEBUG,
    LogLevelVerbose     =   TROG_LEVEL_VERBOSE,
    LogLevelAll         =   TROG_LEVEL_ALL,
};

} // namespace
#endif