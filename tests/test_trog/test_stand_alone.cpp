///
/// Two things
/// -   #define TROG_FILE_LEVEL .... is required before the mpst of the TROG
///     header files are included in order to activiate the appropriate TROG
///     macros. If left out all the TROG macros will be disabled
/// -   It may (?) be helpful to have include <trog/level.hpp> included before the
///     #define as then the component macros such as TROG_LEVEL_DEBUG
///     will be available to your IDE's auto complete/intellisense
///
/// The macro TROG_FILE_LEVEL determines which TROG_ macro calls actually
/// generate code and collect logging data for possible output.
///
/// The final decision on what Trog outputs is make by the call to Trog::set_level()
/// in the main() function below
///
//#define TROG_DISABLE
#include <trog/loglevel.hpp>
#define TROG_FILE_LEVEL (TROG_LEVEL_DEBUG | TROG_LEVEL_TRACE3 | TROG_LEVEL_CTOR )
#include "configure_trog.hpp"

#include <bitset>
#include <boost/filesystem.hpp>

// TROG_SET_GLOBAL_LEVEL(TROG_LEVEL_VERBOSE|Trog::TROG_LEVEL_CTOR|TROG_LEVEL_TRACE3|TROG_LEVEL_TRACE4);


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
    return Trog::testLevelIsActive(level, threshold);
}
#define TCHECK(b) \
	do {	\
		if (b) {std::cout << "OK line:" << __LINE__ << std::endl; } else { std::cout << "Failed line:" << __LINE__ << std::endl; }			\
	} while(0);	


void testBitMask()
{
	using namespace Trog;

	std::vector<long> log_level_values;
	long xerror = static_cast<long>(TROG_LEVEL_ERROR);
	long xwarn = static_cast<long>(TROG_LEVEL_WARN);
	long xinfo = static_cast<long>(TROG_LEVEL_INFO);
	long xdebug = static_cast<long>(TROG_LEVEL_DEBUG);
	long xverbose = static_cast<long>(TROG_LEVEL_VERBOSE);
	long xtotrace = static_cast<long>(TROG_LEVEL_CTOR);
	long xtrace4 = static_cast<long>(TROG_LEVEL_TRACE4);
	long xfdtrace = static_cast<long>(TROG_LEVEL_FDTRACE);
	bool b1 = testLevels(TROG_LEVEL_VERBOSE, TROG_LEVEL_VERBOSE);
	TCHECK(b1);
	bool b2 = testLevels(TROG_LEVEL_INFO, TROG_LEVEL_VERBOSE);
	TCHECK(b2);
	bool b3 = testLevels(TROG_LEVEL_ERROR, TROG_LEVEL_VERBOSE);
	TCHECK(b3);

	bool b4 = testLevels(TROG_LEVEL_VERBOSE, (TROG_LEVEL_VERBOSE | TROG_LEVEL_CTOR));
	TCHECK(b4);
	bool b5 = testLevels(TROG_LEVEL_TRACE4, (TROG_LEVEL_VERBOSE | TROG_LEVEL_CTOR));
	TCHECK(!b5);
	bool b6 = testLevels(TROG_LEVEL_FDTRACE, (TROG_LEVEL_VERBOSE | TROG_LEVEL_CTOR));
	TCHECK(!b6);
	bool b7 = testLevels(TROG_LEVEL_CTOR, (TROG_LEVEL_VERBOSE | TROG_LEVEL_CTOR));
	TCHECK(b7);
	bool b8 = testLevels(TROG_LEVEL_FDTRACE, (TROG_LEVEL_WARN | TROG_LEVEL_CTOR | TROG_LEVEL_FDTRACE));
	TCHECK(b8);

    b8 = testLevels(TROG_LEVEL_ERROR, (TROG_LEVEL_DEBUG | TROG_LEVEL_CTOR | TROG_LEVEL_TRACE3));
    TCHECK(b8);

    b8 = testLevels(TROG_LEVEL_WARN, (TROG_LEVEL_DEBUG | TROG_LEVEL_CTOR | TROG_LEVEL_TRACE3));
    TCHECK(b8);

    b8 = testLevels(TROG_LEVEL_INFO, (TROG_LEVEL_DEBUG | TROG_LEVEL_CTOR | TROG_LEVEL_TRACE3));
    TCHECK(b8);

    b8 = testLevels(TROG_LEVEL_DEBUG, (TROG_LEVEL_DEBUG | TROG_LEVEL_CTOR | TROG_LEVEL_TRACE3));
    TCHECK(b8);

    b8 = testLevels(TROG_LEVEL_VERBOSE, (TROG_LEVEL_DEBUG | TROG_LEVEL_CTOR | TROG_LEVEL_TRACE3));
    TCHECK(!b8);

    b8 = testLevels(TROG_LEVEL_FDTRACE, (TROG_LEVEL_WARN | TROG_LEVEL_CTOR | TROG_LEVEL_TRACE3));
    TCHECK(!b8);

}

void displayBitMask()
{
	using namespace Trog;
    auto x = TROG_LEVEL_ERROR;
    std::cout << std::hex << x << std::endl;
	std::bitset<TROG_BIT_WIDTH> berror(TROG_LEVEL_ERROR);
	std::cout << "Error    : " << berror << std::endl;
	std::bitset<TROG_BIT_WIDTH> bwarn(TROG_LEVEL_WARN);
	std::cout << "Warn     : " << bwarn << std::endl;
	std::bitset<TROG_BIT_WIDTH> binfo(TROG_LEVEL_INFO);
	std::cout << "Info     : " << binfo << std::endl;
	std::bitset<TROG_BIT_WIDTH> bdebug(TROG_LEVEL_DEBUG);
	std::cout << "Debug    : " << bdebug << std::endl;
	std::bitset<TROG_BIT_WIDTH> bverbose(TROG_LEVEL_VERBOSE);
	std::cout << "Verbose  : " << bverbose << std::endl;
	std::bitset<TROG_BIT_WIDTH> btotrace(TROG_LEVEL_CTOR);
	std::cout << "torTrace : " << btotrace << std::endl;
    std::bitset<TROG_BIT_WIDTH> btrace3(TROG_LEVEL_TRACE3);
    std::cout << "Trace4   : " << btrace3 << std::endl;
	std::bitset<TROG_BIT_WIDTH> btrace4(TROG_LEVEL_TRACE4);
	std::cout << "Trace4   : " << btrace4 << std::endl;
    std::bitset<TROG_BIT_WIDTH> btracemax(TROG_TRACE_MAX);
    std::cout << "Tracemax : " << btracemax << std::endl;
	std::bitset<TROG_BIT_WIDTH> bfdtrace(TROG_LEVEL_FDTRACE);
	std::cout << "FDTrace  : " << bfdtrace << std::endl;

}


void testLevelText()
{
	std::cout << "Trogger text " << Trog::LogLevelText(TROG_LEVEL_ERROR) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(TROG_LEVEL_WARN) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(TROG_LEVEL_INFO) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(TROG_LEVEL_DEBUG) << std::endl;
	std::cout << "Trogger text " << Trog::LogLevelText(TROG_LEVEL_VERBOSE) << std::endl;
	std::cout << "Trogger text expected TOR " << Trog::LogLevelText(TROG_LEVEL_CTOR) << std::endl;
	std::cout << "Trogger text expected TRC " << Trog::LogLevelText(TROG_LEVEL_TRACE4) << std::endl;
	std::cout << "Trogger text exoected FD  " << Trog::LogLevelText(TROG_LEVEL_FDTRACE) << std::endl;
}
void test_level_text()
{
	std::string tmp;
	tmp = Trog::LogLevelText(TROG_LEVEL_ERROR);
	TCHECK( (std::string("ERR ") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_WARN);
	TCHECK( (std::string("WRN ") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_INFO);
	TCHECK( (std::string("INF ") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_DEBUG);
	TCHECK( (std::string("DBG ") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_VERBOSE);
	TCHECK( (std::string("VRB ") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_CTOR);
	TCHECK( (std::string("CTOR") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_FDTRACE);
	TCHECK( (std::string("FD__") == tmp));

	tmp = Trog::LogLevelText(TROG_LEVEL_TRACE7);
	TCHECK( (std::string("TRC7") == tmp));

}
void test_this_file()
{
    bool b;
    b= Trog::testLevelIsActive(TROG_LEVEL_ERROR, TROG_FILE_LEVEL);
    TCHECK(b)
    b = Trog::testLevelIsActive(TROG_LEVEL_WARN, TROG_FILE_LEVEL);
    TCHECK(b)
    b = Trog::testLevelIsActive(TROG_LEVEL_INFO, TROG_FILE_LEVEL);
    TCHECK(b)
    b = Trog::testLevelIsActive(TROG_LEVEL_DEBUG, TROG_FILE_LEVEL);
    TCHECK(b)
    b = Trog::testLevelIsActive(TROG_LEVEL_VERBOSE, TROG_FILE_LEVEL);
    TCHECK(!b)
    b = Trog::testLevelIsActive(TROG_LEVEL_CTOR, TROG_FILE_LEVEL);
    TCHECK(b)
    b = Trog::testLevelIsActive(TROG_LEVEL_TRACE3, TROG_FILE_LEVEL);
    TCHECK(b)
    b = Trog::testLevelIsActive(TROG_LEVEL_TRACE4, TROG_FILE_LEVEL);
    TCHECK(!b)
    b = Trog::testLevelIsActive(TROG_LEVEL_FDTRACE, TROG_FILE_LEVEL);
    TCHECK(!b)
    auto x1 = Trog::get_level();
    b = Trog::testLevelIsActive(TROG_LEVEL_DEBUG, x1);
    TCHECK(b)
    Trog::set_level(TROG_LEVEL_INFO);
    auto x2 = Trog::get_level();
    // a few lines earlier this next line produced true
    b = Trog::testLevelIsActive(TROG_LEVEL_DEBUG, x2);
    TCHECK(!b)
}
int main()
{

	displayBitMask();
    testBitMask();
	testLevelText();
	test_level_text();
	test_this_file();
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
}