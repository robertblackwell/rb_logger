#ifndef trog_guard_simple_worker_hpp
#define trog_guard_simple_worker_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {
namespace Simple {
class Writer;

using WriterSPtr = std::shared_ptr<Writer>;

class Writer: public WriterInterface
{
public:

    static WriterSPtr make(FormatterSPtr formatter_sptr, SinkCollection& sinks);

    Writer(FormatterSPtr formatter_sptr, SinkCollection& sinks);
    ~Writer();
    void submit(LogCallDataSPtr log_data_sptr);
    void cleanup();
    void wait();
    void process(LogCallDataSPtr ldata_sptr);

private:
    SinkCollection&                 m_sinks;
    FormatterSPtr                   m_formatter_sptr;
};

} //namespace
} // namespace
#endif