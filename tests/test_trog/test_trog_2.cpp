#include <bitset>
#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <boost/filesystem.hpp>
#include <trog/trog.hpp>


SET_LOGLEVEL(LOG_LEVEL_VERBOSE|LOG_LEVEL_FDTRACE|LOG_LEVEL_TORTRACE|LOG_LEVEL_TRACE)


TEST_CASE("level text")
{
	CHECK(true);
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
	CHECK( (std::string("VRB") == tmp));

	int64_t x1 = Trog::LogLevelCTorTrace;
	tmp = Trog::LogLevelText(Trog::LogLevelCTorTrace);
	CHECK( (std::string("TOR") == tmp));

	int64_t x2= Trog::LogLevelFDTrace;
	tmp = Trog::LogLevelText(Trog::LogLevelFDTrace);
	CHECK( (std::string("FD ") == tmp));

	int64_t x3 = Trog::LogLevelTrace;
	tmp = Trog::LogLevelText(Trog::LogLevelTrace);
	CHECK( (std::string("TRC") == tmp));

	int64_t x4 = Trog::LogLevelTrace3;

	int64_t x5 = Trog::LogLevelTrace4;
	tmp = Trog::LogLevelText(Trog::LogLevelTrace4);
	CHECK( (std::string("TR4") == tmp));
}
TEST_CASE("testLevelForActive")
{
	bool tmp;
	tmp = Trog::testLevelForActive(Trog::LogLevelDebug, Trog::LogLevelInfo);
	tmp = Trog::testLevelForActive(Trog::LogLevelDebug, Trog::LogLevelWarn);

	tmp = Trog::testLevelForActive(Trog::LogLevelDebug, Trog::LogLevelInfo|Trog::LogLevelTrace);
	tmp = Trog::testLevelForActive(Trog::LogLevelDebug, Trog::LogLevelWarn|Trog::LogLevelTrace);

	tmp = Trog::testLevelForActive(Trog::LogLevelTrace, Trog::LogLevelInfo|Trog::LogLevelTrace);
	tmp = Trog::testLevelForActive(Trog::LogLevelCTorTrace, Trog::LogLevelInfo|Trog::LogLevelTrace);
	tmp = Trog::testLevelForActive(Trog::LogLevelFDTrace, Trog::LogLevelInfo|Trog::LogLevelTrace);
}
