#ifndef trog_guard_loglevel_HPP
#define trog_guard_loglevel_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>


namespace Trog {

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
typedef int64_t LogLevelType;
/**
 *  The righthand 8 bits are a bit mask for Trace levels. These are treated like "channels"
 *  in other logging libraries. They are either on or off almost regardless of the prevailing
 *  threshold level. 
 */
const int LevelBitWidth = 64;
const int TraceBits = 8;
enum LogLevelTypeValues: int64_t {
    LogLevelCTorTrace   = 0b00000001,
    LogLevelFDTrace     = 0b00000010,
    LogLevelTrace3      = 0b00000100,
    LogLevelTrace4      = 0b00001000,
    LogLevelTrace5      = 0b00010000,
    LogLevelTrace6      = 0b00100000,
    LogLevelTrace7      = 0b01000000,
    LogLevelTrace8      = 0b10000000,
    /** space for future use*/
    LogLevelError       = 1 << TraceBits,
    LogLevelWarn        = 2 << TraceBits,
    LogLevelInfo        = 3 << TraceBits,
    LogLevelDebug       = 4 << TraceBits,
    LogLevelVerbose     = 5 << TraceBits,
};

/** free functions */
std::string LogLevelText(LogLevelType level);

/** sets the global filter to enabled */
void setEnabled(bool on_off);

/** sets the global threshold */
void setGlobalThreshold(LogLevelType level);

/** sets the threshold for a compilation unti*/
void setFileThreshold(LogLevelType level);

/** tests a loglevel against a threshold level
 *  returns true if the level is active relative to the given threshold
 */
bool testLevelIsActive(LogLevelType level, LogLevelType threshold );

bool levelIsActive(LogLevelType level, LogLevelType file_threshold, LogLevelType global_threshold);


} // namespace
#endif