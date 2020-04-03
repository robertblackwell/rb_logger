#include <boost/filesystem.hpp>
#include <rb_logger/rb_logger.hpp>

#include <bitset>

SET_LOGLEVEL(LOG_LEVEL_VERBOSE|LOG_LEVEL_FDTRACE|LOG_LEVEL_TORTRACE|LOG_LEVEL_TRACE)


class ATestClass
{
	public:
	ATestClass()
	{
		LogTorTrace();
	}
	void doSomething()
	{
		LogTrace("Inside doSomething");
	}
	~ATestClass()
	{
		LogTorTrace();
	}
};

// bool testLevels(RBLogger::LogLevel level, RBLogger::LogLevel threshold )
bool testLevels(long level, long threshold )
{
	long result;
	long threshold_bits;
	std::bitset<8> blevel(level);
	std::bitset<8> bthreshold(threshold);
	std::cout << "testLevels entry level: " << blevel << " threshold: " << bthreshold << std::endl;
	if (level <= 4) {
		threshold_bits = (threshold & 0x07);
		std::bitset<8> bthreshold_bits(threshold);
		result = (level & threshold_bits);
		std::bitset<8> bresult(result);
		std::cout << "testLevels level =< 4 threshold_bits: " << bthreshold_bits << " result: " << bresult << std::endl;
		return result;
	} else {
		return (level <= threshold);
	}
}
#define CHECK(b) \
	do {	\
		if (b) {std::cout << "OK line:" << __LINE__ << std::endl; } else { std::cout << "Failed line:" << __LINE__ << std::endl; }			\
	} while(0);	


void testBitMask()
{
	using namespace RBLogger;

	std::vector<long> log_level_values;
	long xerror = static_cast<long>(RBLogger::LogLevelError);
	long xwarn = static_cast<long>(RBLogger::LogLevelWarn);
	long xinfo = static_cast<long>(RBLogger::LogLevelInfo);
	long xdebug = static_cast<long>(RBLogger::LogLevelDebug);
	long xverbose = static_cast<long>(RBLogger::LogLevelVerbose);
	long xtotrace = static_cast<long>(RBLogger::LogLevelCTorTrace);
	long xtrace = static_cast<long>(RBLogger::LogLevelTrace);
	long xfdtrace = static_cast<long>(RBLogger::LogLevelFDTrace);
	bool b1 = testLevels(LogLevelVerbose, LogLevelVerbose);
	CHECK(b1);
	bool b2 = testLevels(LogLevelInfo, LogLevelVerbose);
	CHECK(b2);
	bool b3 = testLevels(LogLevelError, LogLevelVerbose);
	CHECK(b3);

	bool b4 = testLevels(LogLevelVerbose, (LogLevelVerbose | LogLevelCTorTrace));
	CHECK(b4);
	bool b5 = testLevels(LogLevelTrace, (LogLevelVerbose | LogLevelCTorTrace));
	CHECK(!b5);
	bool b6 = testLevels(LogLevelFDTrace, (LogLevelVerbose | LogLevelCTorTrace));
	CHECK(!b6);
	bool b7 = testLevels(LogLevelCTorTrace, (LogLevelVerbose | LogLevelCTorTrace));
	CHECK(b7);
}

void displayBitMask()
{
	using namespace RBLogger;

	std::bitset<8> berror(RBLogger::LogLevelError); 	std::cout << "Error   : " << berror << std::endl;
	std::bitset<8> bwarn(RBLogger::LogLevelWarn); 		std::cout << "Warn    : " << bwarn << std::endl;
	std::bitset<8> binfo(RBLogger::LogLevelInfo); 		std::cout << "Info    : " << binfo << std::endl;
	std::bitset<8> bdebug(RBLogger::LogLevelDebug); 	std::cout << "Debug   : " << bdebug << std::endl;
	std::bitset<8> bverbose(RBLogger::LogLevelVerbose); std::cout << "Verbose : " << bverbose << std::endl;
	std::bitset<8> btotrace(RBLogger::LogLevelCTorTrace);std::cout << "torTrace: " << btotrace << std::endl;
	std::bitset<8> btrace(RBLogger::LogLevelTrace); 	std::cout << "Trace   : " << btrace << std::endl;
	std::bitset<8> bfdtrace(RBLogger::LogLevelFDTrace); std::cout << "FDTrace : " << bfdtrace << std::endl;


}


void testLevelText()
{
	std::cout << "Logger text " << RBLogger::LogLevelText(RBLogger::LogLevelError) << std::endl;
	std::cout << "Logger text " << RBLogger::LogLevelText(RBLogger::LogLevelWarn) << std::endl;
	std::cout << "Logger text " << RBLogger::LogLevelText(RBLogger::LogLevelInfo) << std::endl;
	std::cout << "Logger text " << RBLogger::LogLevelText(RBLogger::LogLevelDebug) << std::endl;
	std::cout << "Logger text " << RBLogger::LogLevelText(RBLogger::LogLevelVerbose) << std::endl;
	std::cout << "Logger text expected TOR " << RBLogger::LogLevelText(RBLogger::LogLevelCTorTrace) << std::endl;
	std::cout << "Logger text expected TRC " << RBLogger::LogLevelText(RBLogger::LogLevelTrace) << std::endl;
	std::cout << "Logger text exoected FD " << RBLogger::LogLevelText(RBLogger::LogLevelFDTrace) << std::endl;
}

int main()
{
	RBLogger::setEnabled(true);
	RBLogger::enableForLevel(LOG_LEVEL_FDTRACE|LOG_LEVEL_TRACE|LOG_LEVEL_WARN);

	displayBitMask();
	testLevelText();
	LogError("This is an error");
	LogWarn("This is an warning");
	LogInfo("This is info");
	LogDebug("This is debug", "and this is some more", 42);
	LogVerbose("This is verbose");
	ATestClass* aptr = new ATestClass();
	aptr->doSomething();
	ATestClass* bptr = new ATestClass();
	delete aptr;
	delete bptr;
	LogFDTrace(33);
	LogTrace("This is just a trace");
	LogError("This is an error");
	testBitMask();
}