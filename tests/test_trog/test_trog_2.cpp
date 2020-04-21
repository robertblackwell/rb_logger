#include <bitset>
#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <boost/filesystem.hpp>
#include <trog/trog.hpp>

namespace Trog {
using Trogger = Collector<Formatter, SinkDefault, Simple::Writer>;
}

TROG_SET_FILE_LEVEL(Trog::LogLevelVerbose);
TROG_SET_GLOBAL_LEVEL(Trog::LogLevelVerbose|Trog::LogLevelCTorTrace|Trog::LogLevelTrace3|Trog::LogLevelTrace4);


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

	int64_t x3 = Trog::LogLevelTrace3;
	tmp = Trog::LogLevelText(Trog::LogLevelTrace3);
	CHECK( (std::string("TRC3") == tmp));

	int64_t x4 = Trog::LogLevelTrace3;

	int64_t x5 = Trog::LogLevelTrace4;
	tmp = Trog::LogLevelText(Trog::LogLevelTrace4);
	CHECK( (std::string("TRC4") == tmp));
}
TEST_CASE("testLevelForActive")
{
	bool tmp;
	tmp = Trog::testLevelIsActive(Trog::LogLevelDebug, Trog::LogLevelVerbose);
	CHECK(tmp);
	tmp = Trog::testLevelIsActive(Trog::LogLevelDebug, Trog::LogLevelWarn);
	CHECK(!tmp);

	tmp = Trog::testLevelIsActive(Trog::LogLevelDebug, Trog::LogLevelVerbose|Trog::LogLevelTrace3);
	CHECK(tmp);
	tmp = Trog::testLevelIsActive(Trog::LogLevelDebug, Trog::LogLevelWarn|Trog::LogLevelTrace3);
	CHECK(!tmp);

	tmp = Trog::testLevelIsActive(Trog::LogLevelTrace3, Trog::LogLevelInfo|Trog::LogLevelTrace3);
	CHECK(tmp);
	tmp = Trog::testLevelIsActive(Trog::LogLevelCTorTrace, Trog::LogLevelInfo|Trog::LogLevelTrace3);
	CHECK(!tmp);
	tmp = Trog::testLevelIsActive(Trog::LogLevelFDTrace, Trog::LogLevelInfo|Trog::LogLevelTrace3);
	CHECK(!tmp);
	tmp = Trog::testLevelIsActive(Trog::LogLevelTrace3, Trog::LogLevelError|Trog::LogLevelTrace3);
	CHECK(!tmp);
}
