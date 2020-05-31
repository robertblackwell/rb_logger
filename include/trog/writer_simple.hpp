#ifndef trog_guard_worker_simple_hpp
#define trog_guard_worker_simple_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {
namespace Simple {


class Writer: public WriterInterface
{
    SinkInterface*          m_sink_ptr;
    FormatterInterface*     m_formatter_ptr;
    std::mutex              m_write_mutex;
    std::condition_variable m_write_cond_var;
public:
    static std::shared_ptr<Writer> make(FormatterInterface* fmt, SinkInterface* sink)
    {
        return std::make_shared<Writer>(fmt, sink);
    }
    Writer(FormatterInterface* fmt, SinkInterface* sink): m_formatter_ptr(fmt), m_sink_ptr(sink)
    {}
    ~Writer()
    {}
    void submit(LogCallDataSPtr log_data_sptr)
    {
        process(log_data_sptr);
    }
    void cleanup()
    {}
    void wait()
    {}
    void process(LogCallDataSPtr ldata_sptr)
    {
        // protect the writes from being interleaved with other writers
        std::lock_guard<std::mutex> lock{m_write_mutex};
        std::string output = m_formatter_ptr->format(ldata_sptr);
        m_sink_ptr->write_output(output);
    }

};


} //namespace
} // namespace

#endif