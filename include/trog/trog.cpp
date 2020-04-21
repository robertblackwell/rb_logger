//
#include <iostream>
#include <sstream>
#include <stdarg.h>
#include <bitset>
#include <cstdint>
#include <trog/trog.hpp>

bool Trog::logger_enabled = true;
Trog::LogLevelType Trog::Trogger::allEnabled = 
    Trog::LogLevelVerbose | Trog::LogLevelFDTrace | Trog::LogLevelTrace | Trog::LogLevelCTorTrace;

Trog::LogLevelType Trog::Trogger::globalThreshold = Trog::Trogger::allEnabled; 

Trog::Trogger Trog::Trogger::activeTrogger{};


void Trog::setEnabled(bool on_off)
{
    logger_enabled = on_off;
}
void Trog::enableForLevel(LogLevelType level)
{
    Trog::Trogger::globalThreshold  = level;
    logger_enabled = true;
}

bool Trog::Trogger::enabled()
{
    /// this function is only used for Trace functions
    /// we want these active with DEBUG levels
    LogLevelType lvl = LOG_LEVEL_DEBUG;
    LogLevelType tmp = globalThreshold;
    return ( ((int)lvl <= (int)tmp) && Trog::logger_enabled );
}

std::string Trog::LogLevelText(Trog::LogLevelType level)
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
            case Trog::LogLevelTrace:
                return "TRC";
                break;
            case Trog::LogLevelTrace2:
                return "TR2";
                break;
            case Trog::LogLevelTrace3:
                return "TR3";
                break;
            case Trog::LogLevelTrace4:
                return "TR4";
                break;
            default:
                assert(false);
        }
    }
}

bool Trog::testLevelForActive(long level, long threshold )
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

bool Trog::levelIsActive(LogLevelType lvl, LogLevelType threshold)
{
    if (! Trog::logger_enabled)
        return false;
    if (testLevelForActive(lvl, threshold)) {
        if (testLevelForActive(lvl, globalThreshold)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
    /// use the lowest threshold - local or global
    LogLevelType tmp = (threshold <= globalThreshold) ? threshold : globalThreshold;
    return ( ((int)lvl <= (int)tmp) && Trog::logger_enabled );
//    return ( ((int)lvl <= (int)threshold) && Trog::logger_enabled );
}
