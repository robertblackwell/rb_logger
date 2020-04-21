#include <bitset>
#include <string>
// #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// #include <doctest/doctest.h>
#include <boost/filesystem.hpp>

#include <trog/loglevel.hpp>
#include <trog/call_data.hpp>
#include <trog/formatter.hpp>
#include <trog/collector.hpp>
#include <trog/writer_threaded.hpp>
#include <trog/writer_simple.hpp>
#include <trog/macros.hpp>

static Trog::LogLevelType TrogFileLevel = Trog::LogLevelVerbose | Trog::LogLevelCTorTrace;
static Trog::LogLevelType TrogGlobalLevel = Trog::LogLevelVerbose | Trog::LogLevelCTorTrace;

struct ATestClass
{
	ATestClass(int i)
	{
		TROG_TRACE_CTOR();
		std::cout << __func__ << "i: " <<  i << std::endl;
	}
	~ATestClass()
	{
		TROG_TRACE_CTOR();
	}
};

void test_case()
{
	using namespace Trog;

	auto s1 = new Sinkstdout();
	auto s2 = new Sinkstderr();
	auto s3 = Sink2<Sinkstdout, Sinkstderr>::make();
	auto s32 = std::make_shared<SinkFileT<SinkFileName> >();
	auto s321 = SinkFile::make();
	s32->write_output("");
	s321->write_output("");
	auto s31 = SinkFileT<Trog::SinkFileName>::make();
	auto s34 = std::make_shared<Trog::SinkFileT<Trog::SinkFileName> >();
	auto s341 = SinkDefault::make();
	s34->write_output("");
	s3->write_output("");
	s341->write_output("");
	using DefaultCollector = Collector<Formatter, SinkDefault, Simple::Writer>;
	DefaultCollector& collector = DefaultCollector::getInstance();
	for(int i = 0; i < 10; i++) {
		std::this_thread::sleep_for (std::chrono::seconds(1));
		std::string s1("This is the first string");
		std::string s2("This is the second string");
		collector.collect(
			Trog::LogLevelDebug, 
			Trog::LogLevelVerbose,
			Trog::LogLevelVerbose,
			"default",
			__FILE__,
			__FUNCTION__,
			__LINE__,
			s1,s1

		);
		TROG_COLLECT(Trog::LogLevelVerbose, "One", "Two");
		TROG_WARN("THis is a warning");
		ATestClass a(i);

	}
	// collector.wait();
	std::cout << "leaving test function" << std::endl;
}
int main()
{
	try {
	test_case();
	} catch(...){
		std::cout << "Main catch" << std::endl;
	}
	std::cout << "Leaving main" << std::endl;
}
#if 0
TEST_CASE("formatter")
{
	Trog::FormatterSPtr formatter = std::make_shared<Trog::Formatter>();

}


void initializeLogger(
	Trog::FormatterSPtr fsptr, 
	std::vector<std::string> collector_names, 
	std::vector<Trog::SinkInterfaceSPtr> sinks_sptr
	)
{
	Trog::FormatterSPtr formatter = std::make_shared<Trog::Formatter>();
	
	Trog::Thread::WriterSPtr worker_sptr = std::make_shared<Trog::Thread::Writer>(formatter, sinks_sptr);
	std::vector<Trog::CollectorSPtr> collectors;
	for(auto name : collector_names) {
		collectors.push_back(std::make_shared<Trog::Collector>(name, worker_sptr, fsptr));
	}
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
	Trog::WriterInterfaceSPtr worker_sptr = std::make_shared<Trog::Thread::Writer>(formatter, collection2);
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
#endif