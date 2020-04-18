#ifndef trog_guard_worker_hpp
#define trog_guard_worker_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/trog_formatter.hpp>
#include <trog/trog_call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {
class Worker;

using WorkerSPtr = std::shared_ptr<Worker>;

class Worker
{
    public:

    Worker(FormatterSPtr formatter_sptr, SinkCollection& sinks): m_sinks(sinks)
    {
        m_formatter_sptr = formatter_sptr;
        std::set_terminate([](){

        });
        m_thread_object_uptr = std::make_unique<std::thread>([this]() 
        {
            this->loop();
        });
    }
    ~Worker()
    {
        std::cout << "worker destructor" << "queue size : " << m_logdata_queue.size() << std::endl;
    }
    void submit(LogCallDataSPtr log_data_sptr)
    {

    }
    void cleanup()
    {
        
    }

    void addToQueue(LogCallDataSPtr log_data_sptr)
    {
        {
            std::lock_guard<std::mutex> lock{m_queue_mutex};
            m_logdata_queue.push(log_data_sptr);
            m_queue_cond_var.notify_one();
        }
    }
    void loop()
    {
        for(;;) {
            // std::this_thread::sleep_for (std::chrono::seconds(5));
            std::cout << "Worker loop Q size: " << m_logdata_queue.size()  << std::endl;
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
    void wait()
    {
        m_thread_object_uptr->join();
    }
    void process(LogCallDataSPtr ldata_sptr)
    {
        std::string output = m_formatter_sptr->format(ldata_sptr);
        for(int index = 0; index < m_sinks.size(); index++) {
            m_sinks[index]->write(output);
        }
    }
    private:
    SinkCollection&                 m_sinks;
    FormatterSPtr                   m_formatter_sptr;
    std::queue<LogCallDataSPtr>     m_logdata_queue;
    std::mutex                      m_queue_mutex;
    std::condition_variable         m_queue_cond_var;
    std::unique_ptr<std::thread>    m_thread_object_uptr;
};
} //namespace
#endif