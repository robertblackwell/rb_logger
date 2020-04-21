#include <trog/worker_thread.hpp>

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {
namespace Thread {

Writer::Writer(FormatterSPtr formatter_sptr, SinkCollection& sinks): m_sinks(sinks)
{
    m_formatter_sptr = formatter_sptr;
    std::set_terminate([](){

    });
    m_thread_object_uptr = std::make_unique<std::thread>([this]() 
    {
        this->loop();
    });
}
Writer::~Writer()
{
    std::cout << "worker destructor" << "queue size : " << m_logdata_queue.size() << std::endl;
}
void Writer::submit(LogCallDataSPtr log_data_sptr)
{

}
void Writer::cleanup()
{

}

void Writer::addToQueue(LogCallDataSPtr log_data_sptr)
{
    {
        std::lock_guard<std::mutex> lock{m_queue_mutex};
        m_logdata_queue.push(log_data_sptr);
        m_queue_cond_var.notify_one();
    }
}
void Writer::loop()
{
    for(;;) {
        // std::this_thread::sleep_for (std::chrono::seconds(5));
        std::cout << "Writer loop Q size: " << m_logdata_queue.size()  << std::endl;
        #if 1
        LogCallDataSPtr ldata_sptr;
        {
            std::unique_lock<std::mutex> lock{m_queue_mutex};
            m_queue_cond_var.wait(lock, [this]()
            {
                return !m_logdata_queue.empty();
            });
            ldata_sptr = m_logdata_queue.front();
            m_logdata_queue.pop();
        }
        process(ldata_sptr);
        #endif
    }
}
void Writer::wait()
{
    m_thread_object_uptr->join();
}
void Writer::process(LogCallDataSPtr ldata_sptr)
{
    std::string output = m_formatter_sptr->format(ldata_sptr);
    for(int index = 0; index < m_sinks.size(); index++) {
        m_sinks[index]->write(output);
    }
}

WriterSPtr Writer::make(FormatterSPtr formatter_sptr, SinkCollection sinks)
{
    return std::make_shared<Writer>(formatter_sptr, sinks);
}

} //namespace
} // namespace
