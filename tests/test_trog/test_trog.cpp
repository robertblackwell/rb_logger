#include <bitset>
#include <doctest/doctest.h>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <boost/filesystem.hpp>
#include <trog/trog.hpp>

namespace Trog {
using Trogger = Collector<Formatter, SinkDefault, Simple::Writer>;
}

TROG_SET_FILE_LEVEL(Trog::LogLevelVerbose);
// TROG_SET_GLOBAL_LEVEL(Trog::LogLevelVerbose|Trog::LogLevelCTorTrace|Trog::LogLevelTrace3|Trog::LogLevelTrace4);


class ATestClass
{
	public:
	ATestClass()
	{
		TROG_TRACE_CTOR();
	}		
	void doSomething()
	{
		TROG_DEBUG("Inside doSomething");
	}
	~ATestClass()
	{
		TROG_TRACE_CTOR();
	}
};

// bool testLevels(Trog::LogLevel level, Trog::LogLevel threshold )
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
#define TCHECK(b) \
	do {	\
		if (b) {std::cout << "OK line:" << __LINE__ << std::endl; } else { std::cout << "Failed line:" << __LINE__ << std::endl; }			\
	} while(0);	


void testBitMask()
{
	using namespace Trog;

	std::vector<long> log_level_values;
	long xerror = static_cast<long>(Trog::LogLevelError);
	long xwarn = static_cast<long>(Trog::LogLevelWarn);
	long xinfo = static_cast<long>(Trog::LogLevelInfo);
	long xdebug = static_cast<long>(Trog::LogLevelDebug);
	long xverbose = static_cast<long>(Trog::LogLevelVerbose);
	long xtotrace = static_cast<long>(Trog::LogLevelCTorTrace);
	long xtrace4 = static_cast<long>(Trog::LogLevelTrace4);
	long xfdtrace = static_cast<long>(Trog::LogLevelFDTrace);
	bool b1 = testLevels(LogLevelVerbose, LogLevelVerbose);
	TCHECK(b1);
	bool b2 = testLevels(LogLevelInfo, LogLevelVerbose);
	TCHECK(b2);
	bool b3 = testLevels(LogLevelError, LogLevelVerbose);
	TCHECK(b3);

	bool b4 = testLevels(LogLevelVerbose, (LogLevelVerbose | LogLevelCTorTrace));
	TCHECK(b4);
	bool b5 = testLevels(LogLevelTrace4, (LogLevelVerbose | LogLevelCTorTrace));
	TCHECK(!b5);
	bool b6 = testLevels(LogLevelFDTrace, (LogLevelVerbose | LogLevelCTorTrace));
	TCHECK(!b6);
	bool b7 = testLevels(LogLevelCTorTrace, (LogLevelVerbose | LogLevelCTorTrace));
	TCHECK(b7);
	bool b8 = testLevels(LogLevelFDTrace, (LogLevelWarn | LogLevelCTorTrace | LogLevelFDTrace));
	TCHECK(b8);
}

void displayBitMask()
{
	using namespace Trog;

	std::bitset<8> berror(Trog::LogLevelError); 	std::cout << "Error   : " << berror << std::endl;
	std::bitset<8> bwarn(Trog::LogLevelWarn); 		std::cout << "Warn    : " << bwarn << std::endl;
	std::bitset<8> binfo(Trog::LogLevelInfo); 		std::cout << "Info    : " << binfo << std::endl;
	std::bitset<8> bdebug(Trog::LogLevelDebug); 	std::cout << "Debug   : " << bdebug << std::endl;
	std::bitset<8> bverbose(Trog::LogLevelVerbose); std::cout << "Verbose : " << bverbose << std::endl;
	std::bitset<8> btotrace(Trog::LogLevelCTorTrace);std::cout << "torTrace: " << btotrace << std::endl;
	std::bitset<8> btrace(Trog::LogLevelTrace4); 	std::cout << "Trace   : " << btrace << std::endl;
	std::bitset<8> bfdtrace(Trog::LogLevelFDTrace); std::cout << "FDTrace : " << bfdtrace << std::endl;


}


void testLevelText()
{
	std::cout << "Trogger text " << Trog::LogLevelText(Trog::LogLevelError) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(Trog::LogLevelWarn) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(Trog::LogLevelInfo) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(Trog::LogLevelDebug) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(Trog::LogLevelVerbose) << std::endl;
	std::cout << "Trogger text expected TOR " << Trog::LogLevelText(Trog::LogLevelCTorTrace) << std::endl;
	std::cout << "Trogger text expected TRC " << Trog::LogLevelText(Trog::LogLevelTrace4) << std::endl;
	std::cout << "Trogger text exoected FD " << Trog::LogLevelText(Trog::LogLevelFDTrace) << std::endl;
}
TEST_CASE("level text")
{
	std::string tmp;
	tmp = Trog::LogLevelText(Trog::LogLevelError);
	CHECK( (std::string("ERR") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelWarn);
	CHECK( (std::string("WRN") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelInfo);
	CHECK( (std::string("INF") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelDebug);
	CHECK( (std::string("DBG") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelVerbose);
	CHECK( (std::string("VER") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelCTorTrace);
	CHECK( (std::string("TOR") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelFDTrace);
	CHECK( (std::string("FD ") == tmp));

	tmp = Trog::LogLevelText(Trog::LogLevelTrace7);
	CHECK( (std::string("TRC") == tmp));

}
int notmain()
{

	displayBitMask();
	testLevelText();
	TROG_ERROR("This is an error");
	TROG_WARN("This is an warning");
	TROG_INFO("This is info");
	TROG_DEBUG("This is debug", "and this is some more", 42);
	TROG_VERBOSE("This is verbose");
	ATestClass* aptr = new ATestClass();
	aptr->doSomething();
	ATestClass* bptr = new ATestClass();
	delete aptr;
	delete bptr;
	TROG_TRACE_FD(33);
	TROG_TRACE3("This is just a trace");
	TROG_ERROR("This is an error");
	testBitMask();
}