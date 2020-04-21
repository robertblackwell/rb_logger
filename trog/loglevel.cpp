//
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <trog/loglevel.hpp>

namespace Trog{
std::string LogLevelText(Trog::LogLevelType level)
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

bool testLevelIsActive(LogLevelType level, LogLevelType threshold )
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

bool levelIsActive(LogLevelType level, LogLevelType file_threshold, LogLevelType global_threshold)
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
} // namespace