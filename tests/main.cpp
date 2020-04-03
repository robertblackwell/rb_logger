#include <boost/filesystem.hpp>
#include <rb_logger/rb_logger.hpp>
#include <bitset>

SET_LOGLEVEL(LOG_LEVEL_MAX)


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

// bool testLevels(RBLogging::LogLevel level, RBLogging::LogLevel threshold )
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
	using namespace RBLogging;

	std::vector<long> log_level_values;
	long xerror = static_cast<long>(RBLogging::LogLevel::error);
	long xwarn = static_cast<long>(RBLogging::LogLevel::warn);
	long xinfo = static_cast<long>(RBLogging::LogLevel::info);
	long xdebug = static_cast<long>(RBLogging::LogLevel::debug);
	long xverbose = static_cast<long>(RBLogging::LogLevel::verbose);
	long xtotrace = static_cast<long>(RBLogging::LogLevel::tortrace);
	long xtrace = static_cast<long>(RBLogging::LogLevel::trace);
	long xfdtrace = static_cast<long>(RBLogging::LogLevel::fdtrace);
	bool b1 = testLevels(LogLevel::verbose, LogLevel::verbose);
	CHECK(b1);
	bool b2 = testLevels(LogLevel::info, LogLevel::verbose);
	CHECK(b2);
	bool b3 = testLevels(LogLevel::error, LogLevel::verbose);
	CHECK(b3);

	bool b4 = testLevels(LogLevel::verbose, (LogLevel::verbose | LogLevel::tortrace));
	CHECK(b4);
	bool b5 = testLevels(LogLevel::trace, (LogLevel::verbose | LogLevel::tortrace));
	CHECK(!b5);
	bool b6 = testLevels(LogLevel::fdtrace, (LogLevel::verbose | LogLevel::tortrace));
	CHECK(!b6);
	bool b7 = testLevels(LogLevel::tortrace, (LogLevel::verbose | LogLevel::tortrace));
	CHECK(b7);
}

void displayBitMask()
{
	using namespace RBLogging;

	std::bitset<8> berror(RBLogging::LogLevel::error); 	std::cout << "Error   : " << berror << std::endl;
	std::bitset<8> bwarn(RBLogging::LogLevel::warn); 		std::cout << "Warn    : " << bwarn << std::endl;
	std::bitset<8> binfo(RBLogging::LogLevel::info); 		std::cout << "Info    : " << binfo << std::endl;
	std::bitset<8> bdebug(RBLogging::LogLevel::debug); 	std::cout << "Debug   : " << bdebug << std::endl;
	std::bitset<8> bverbose(RBLogging::LogLevel::verbose); std::cout << "Verbose : " << bverbose << std::endl;
	std::bitset<8> btotrace(RBLogging::LogLevel::tortrace);std::cout << "torTrace: " << btotrace << std::endl;
	std::bitset<8> btrace(RBLogging::LogLevel::trace); 	std::cout << "Trace   : " << btrace << std::endl;
	std::bitset<8> bfdtrace(RBLogging::LogLevel::fdtrace); std::cout << "FDTrace : " << bfdtrace << std::endl;


}


void testLevelText()
{

	std::cout << "Logger text " << RBLogging::LogLevelText(RBLogging::LogLevel::error) << std::endl;
	std::cout << "Logger text " << RBLogging::LogLevelText(RBLogging::LogLevel::warn) << std::endl;
	std::cout << "Logger text " << RBLogging::LogLevelText(RBLogging::LogLevel::info) << std::endl;
	std::cout << "Logger text " << RBLogging::LogLevelText(RBLogging::LogLevel::debug) << std::endl;
	std::cout << "Logger text " << RBLogging::LogLevelText(RBLogging::LogLevel::verbose) << std::endl;
	std::cout << "Logger text expected TOR " << RBLogging::LogLevelText(RBLogging::LogLevel::tortrace) << std::endl;
	std::cout << "Logger text expected TRC " << RBLogging::LogLevelText(RBLogging::LogLevel::trace) << std::endl;
	std::cout << "Logger text exoected FD " << RBLogging::LogLevelText(RBLogging::LogLevel::fdtrace) << std::endl;


}

int main()
{
	RBLogging::setEnabled(true);
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