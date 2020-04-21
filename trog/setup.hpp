#ifndef trog_guard_setup_hpp
#define trog_guard_setup_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/collector.hpp>
#include <trog/sink.hpp>
#include <trog/writer.hpp>
namespace Trog {

class Trog
{
public:
	static Trog& getInstance()
	{
		static Trog instance;
		return instance;
	}
	void registerCollector(std::string name, CollectorSPtr csptr)
	{
	    m_collectors[name] = csptr;
	}
	CollectorSPtr getCollector(std::string name)
	{
		std::map<std::string, CollectorSPtr>::iterator x;
		if ((x = m_collectors.find(name)) == m_collectors.end()) {
			return nullptr;
		} else {
			return (*x).second;
		}
		return nullptr;		
	}

	void addSink(SinkInterfaceSPtr sink_sptr)
	{
		m_sinks.push_back(sink_sptr);
	}
	void addCollector(std::string name)
	{
		if (m_formatter_sptr == nullptr) {
			throw "cannot add collector until Trog has a formatter";
		}
		if (m_worker_sptr == nullptr) {
			throw "cannot add collector until Trog has a worker";
		}
		registerCollector(name, Collector::make(name, m_worker_sptr, m_formatter_sptr));
	}

	void makeWriter(Simple::WriterSPtr& wsptr)
	{
		if (m_formatter_sptr == nullptr) {
			throw "cannot create worker as no formatter has been made";
		}
		if (m_sinks.size() == 0) {
			throw "cannot create worker, no sinks defined";
		}
		wsptr = Simple::Writer::make(m_formatter_sptr, m_sinks);
	}

	void makeWriter(Thread::WriterSPtr& wsptr)
	{
		if (m_formatter_sptr == nullptr) {
			throw "cannot create worker as no formatter has been made";
		}
		if (m_sinks.size() == 0) {
			throw "cannot create worker, no sinks defined";
		}
		wsptr = Thread::Writer::make(m_formatter_sptr, m_sinks);
	}
	void done()
	{
		if (m_formatter_sptr == nullptr) {
			throw "cannot be done no formatter has been made";
		}
		if (m_sinks.size() == 0) {
			throw "cannot be done  no sinks defined";
		}
		if (m_worker_sptr == nullptr) {
			throw "cannot be done no worker is defined";
		}
	}

	/** coonfigures the objects needed to run a simple non threaded
	 * logger that writes only to the console
	*/
	void configure()
	{
		m_formatter_sptr = Formatter::make();

		SinkstdoutSPtr sink = Sinkstdout::make();
		addSink(sink);

		m_worker_sptr = Simple::Writer::make(m_formatter_sptr, m_sinks);
		CollectorSPtr c = Collector::make("default", m_worker_sptr, m_formatter_sptr);
		registerCollector("default", c);
	}
	/** sets up a logging system that writes to the console and a file
	 * and has a collectors for each of the arguments plus one named 
	 * "default"
	*/
	template<typename T, typename... Ts>
	void configure(std::string file_name, T t, Ts const... xs)
	{
		m_formatter_sptr = Formatter::make();
		SinkstdoutSPtr sink_stdout = Sinkstdout::make();
		addSink(sink_stdout);
		SinkFileSPtr sink_file = SinkFile::make(file_name);
		addSink(sink_file);

		m_worker_sptr = Thread::Writer::make(m_formatter_sptr, m_sinks);
		auto x = Collector::make(t, m_worker_sptr, m_formatter_sptr);
		registerCollector(t, x);
		configure(Ts const... xs)
	}
	template<typename T, typename... Ts>
	void configure(T t, Ts const... xs)
	{
		auto x = Collector::make(t);
		registerCollector(t, x);
		configure(Ts const... xs)
	}

	template<typename T>
	void configure(T t)
	{
		auto x = Collector::make(t);
		registerCollector(t, x);
	}
	/* general purpose setup functions that will configure a threaded logger that writes 
	* to a set of sinks and collects log information from a list of collectors
	* to both the console
	*/
	void configure(
		std::vector<std::string> collector_names, 
		std::vector<SinkInterfaceSPtr> sinks_sptr
		)
	{
		m_formatter_sptr = Formatter::make();
		Thread::WriterSPtr worker_sptr = std::make_shared<Thread::Writer>(m_formatter_sptr, sinks_sptr);
		std::vector<CollectorSPtr> collectors;
		for(auto name : collector_names) {
			auto coll = Collector::make(name, worker_sptr, m_formatter_sptr);
			collectors.push_back(coll);
			this->registerCollector(name, coll);
		}
	}


private:
	Trog()
	{
		m_formatter_sptr = Formatter::make();
	}
  	~Trog()= default;
  	Trog(const Trog&)= delete;
  	Trog& operator=(const Trog&)= delete;

	WriterInterfaceSPtr 				 m_worker_sptr;
	FormatterSPtr						 m_formatter_sptr;
	std::map<std::string, CollectorSPtr> m_collectors;
	std::vector<SinkInterfaceSPtr>		 m_sinks;

}; // class


} // namespace
#endif