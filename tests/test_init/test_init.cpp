#include <bitset>
#include <string>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <boost/filesystem.hpp>
#include <trog/trog.hpp>
#include <trog/trog_call_data.hpp>
#include <trog/trog_formatter.hpp>
#include <trog/trog_collector.hpp>
#include <trog/sink.hpp>
#include <trog/worker.hpp>

SET_LOGLEVEL(LOG_LEVEL_VERBOSE|LOG_LEVEL_FDTRACE|LOG_LEVEL_TORTRACE|LOG_LEVEL_TRACE)

TEST_CASE("crate sinks")
{
	auto s1 = new Trog::Sinkstdout();
	auto s2 = new Trog::Sinkstderr();
	auto s3 = Trog::SinkFile::make("./a_file");

}
TEST_CASE("formatter")
{
	Trog::FormatterSPtr formatter = std::make_shared<Trog::Formatter>();

}
TEST_CASE("level text")
{
	std::string s1 = "this is some stuf for the logger to print";
	Trog::FormatterSPtr formatter = std::make_shared<Trog::Formatter>();
	std::vector<Trog::SinkInterfaceSPtr> collection;
	Trog::SinkstdoutSPtr stdout_sptr = Trog::Sinkstdout::make();
	Trog::SinkInterfaceSPtr ifsptr = stdout_sptr;
	collection.push_back(stdout_sptr);
	std::vector<Trog::SinkInterfaceSPtr> collection2{
		{
			Trog::Sinkstdout::make(),
			Trog::SinkFile::make("./some_file")
		}
	};
	Trog::WorkerSPtr worker_sptr = std::make_shared<Trog::Worker>(formatter, collection2);
	Trog::CollectorSPtr collector1 = Trog::Collector::make("default", worker_sptr, formatter);

	for(;;) {
		std::this_thread::sleep_for (std::chrono::seconds(1));

		collector1->collect(
			Trog::LogLevelDebug, 
			Trog::LogLevelDebug,
			"default",
			__FILE__,
			__FUNCTION__,
			__LINE__,
			s1,s1

		);
	}
	worker_sptr->wait();
}
