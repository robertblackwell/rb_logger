#ifndef trog_guard_loglevel_impl_HPP
#define trog_guard_loglevel_impl_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <bitset>
#include <cstdint>
#include <assert.h>
///
///
///
namespace Trog {

inline std::string LogLevelText(Trog::LogLevelType level)
{
    static std::string tab[] = {
        "",
        "ERR ",
        "WRN ",
        "INF ",
        "DBG ",
        "VRB ",
    };
    static std::string other_tab[] = {
        "BAD1",
        "TRC","TOR",
        "BAD3","FD "
    };
    const int bitWidth = 64;
    const int64_t traceMax = TROG_TRACE_MASK;
    const int64_t traceMask = TROG_TRACE_MASK;
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
            case TROG_LEVEL_CTOR:
                return "CTOR";
                break;
            case TROG_LEVEL_FDTRACE:
                return "FD__";
                break;
            case TROG_LEVEL_TRACE3:
                return "TRC3";
                break;
            case TROG_LEVEL_TRACE4:
                return "TRC4";
                break;
            case TROG_LEVEL_TRACE5:
                return "TRC5";
                break;
            case TROG_LEVEL_TRACE6:
                return "TRC6";
                break;
            case TROG_LEVEL_TRACE7:
                return "TRC7";
                break;
            case TROG_LEVEL_TRACE8:
                return "TRC8";
                break;
            default:
                assert(false);
        }
    }
}

inline bool testLevelIsActive(LogLevelType level, LogLevelType threshold )
{
    const int bitWidth = TraceBits;
    const int32_t traceMax =  TROG_TRACE_MASK;
    const int32_t traceMask = TROG_TRACE_MASK;
	int32_t result;
	int32_t threshold_bits;
	std::bitset<bitWidth> blevel(level);
	std::bitset<bitWidth> bthreshold(threshold);
	// std::cout << "testLevels entry level: " << blevel << " threshold: " << bthreshold << std::endl;
	if (level <= traceMax) {
        // then level is only one of the trace levels
        if ((threshold >= LogLevelError) && (threshold < LogLevelWarn)) {
            // the level is Error maybe plus some trace stuff
            return false;
        }
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
        // return true;
        if (testLevelIsActive(level, global_threshold)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

} // namespace
#endif