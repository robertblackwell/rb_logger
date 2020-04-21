#ifndef trog_guard_loglevel_HPP
#define trog_guard_loglevel_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <bitset>
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
#ifdef NOTINLINE
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
#else
inline std::string LogLevelText(Trog::LogLevelType level)
{
    static std::string tab[] = {
        "",
        "ERR",
        "WRN",
        "INF",
        "DBG",
        "VRB",
    };
    static std::string other_tab[] = {
        "BAD1",
        "TRC","TOR",
        "BAD3","FD "
    };
    const int bitWidth = 64;
    const int64_t traceMax = 0b10000000;
    const int64_t traceMask = 0b11111111;
    int64_t adjusted_level;
    int64_t level_long = level;

    if (level > traceMax) {
        // the level value is not a trace value
        std::bitset<bitWidth> blevel(level);
        adjusted_level = level >> Trog::TraceBits;
        std::bitset<bitWidth> badjusted_level(adjusted_level);
        // std::cout << "LogLevelText level: " << blevel << " adjusted_level : " << badjusted_level << std::endl;
        assert(adjusted_level <= (Trog::LogLevelVerbose >> Trog::TraceBits));
        return tab[adjusted_level];
    } else {
        adjusted_level = (level & traceMask);
        switch(adjusted_level) {
            case Trog::LogLevelCTorTrace:
                return "TOR";
                break;
            case Trog::LogLevelFDTrace:
                return "FD ";
                break;
            case Trog::LogLevelTrace3:
                return "TRC3";
                break;
            case Trog::LogLevelTrace4:
                return "TRC4";
                break;
            case Trog::LogLevelTrace5:
                return "TRC5";
                break;
            case Trog::LogLevelTrace6:
                return "TRC6";
                break;
            case Trog::LogLevelTrace7:
                return "TRC7";
                break;
            case Trog::LogLevelTrace8:
                return "TRC8";
                break;
            default:
                assert(false);
        }
    }
}

inline bool testLevelIsActive(LogLevelType level, LogLevelType threshold )
{
    const int bitWidth = 32;
    const int32_t traceMax = 0b10000000;
    const int32_t traceMask = 0b11111111;
	int32_t result;
	int32_t threshold_bits;
	std::bitset<bitWidth> blevel(level);
	std::bitset<bitWidth> bthreshold(threshold);
	// std::cout << "testLevels entry level: " << blevel << " threshold: " << bthreshold << std::endl;
	if (level <= traceMax) {
		threshold_bits = (threshold & traceMask);
		std::bitset<bitWidth> bthreshold_bits(threshold);
		result = (level & threshold_bits);
		std::bitset<bitWidth> bresult(result);
		// std::cout << "testLevels level =< 4 threshold_bits: " << bthreshold_bits << " result: " << bresult << std::endl;
		return result;
	} else {
		return (level <= threshold);
	}
}

inline bool levelIsActive(LogLevelType level, LogLevelType file_threshold, LogLevelType global_threshold)
{

    if (testLevelIsActive(level, file_threshold)) {
        if (testLevelIsActive(level, global_threshold)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}
#endif

} // namespace
#endif