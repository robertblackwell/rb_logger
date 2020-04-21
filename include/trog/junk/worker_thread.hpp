#ifndef trog_guard_worker_thread_hpp
#define trog_guard_worker_thread_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {
namespace Thread {
class Writer;

using WriterSPtr = std::shared_ptr<Writer>;

class Writer: public WriterInterface
{
public:

    static WriterSPtr make(std::shared_ptr<Formatter> formatter_sptr, std::vector<std::shared_ptr<SinkInterface>> sinks);

    Writer(FormatterSPtr formatter_sptr, SinkCollection& sinks);
    ~Writer();
    void submit(LogCallDataSPtr log_data_sptr);
    void cleanup();
    void addToQueue(LogCallDataSPtr log_data_sptr);
    void loop();
    void wait();
    void process(LogCallDataSPtr ldata_sptr);
private:
    SinkCollection&                 m_sinks;
    FormatterSPtr                   m_formatter_sptr;
    std::queue<LogCallDataSPtr>     m_logdata_queue;
    std::mutex                      m_queue_mutex;
    std::condition_variable         m_queue_cond_var;
    std::unique_ptr<std::thread>    m_thread_object_uptr;
};

} //namespace
} // namespace
#endif