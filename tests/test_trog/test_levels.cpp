#define TROG_ENABLE_FILE_LEVEL
#define TROG_ENABLE_GLOBAL_LEVEL
#include <trog/trog.hpp>

namespace Trog {
using Trogger = Collector<Formatter, SinkDefault, Simple::Writer>;
}

TROG_SET_FILE_LEVEL(Trog::LogLevelVerbose);
TROG_SET_GLOBAL_LEVEL(Trog::LogLevelVerbose|Trog::LogLevelCTorTrace|Trog::LogLevelTrace3|Trog::LogLevelTrace4);

#include <bitset>
#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <boost/filesystem.hpp>

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
