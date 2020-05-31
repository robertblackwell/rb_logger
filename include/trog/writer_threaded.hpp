#ifndef trog_guard_worker_threaded_hpp
#define trog_guard_worker_threaded_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>
#include <trog/writer_interface.hpp>

namespace Trog {
namespace Thread {

class Writer: public WriterInterface
{
    Formatter*                      m_formatter_ptr;
    SinkInterface*                  m_sink_ptr;
    std::queue<LogCallDataSPtr>     m_logdata_queue;
    std::mutex                      m_queue_mutex;
    std::condition_variable         m_queue_cond_var;
    std::unique_ptr<std::thread>    m_thread_object_uptr;
    bool                            m_stop_flag;
    long                            m_max_queue_size;
public:

    static std::shared_ptr<Writer> make(Formatter* fmt, SinkInterface* sink)
    {
        return std::make_shared<Writer>(fmt, sink);
    }


    Writer(Formatter* fmt, SinkInterface* sink): m_formatter_ptr(fmt), m_sink_ptr(sink)
    {
        // std::set_terminate([](){
        //     std::cout << "Inside Writer::terminate" << std::endl;
        // });
        m_stop_flag = false;
        std::atexit([](){
            std::cout << "Inside Writer::exit handler" << std::endl;
        });
        m_thread_object_uptr = std::make_unique<std::thread>([this]()
        {
            this->loop();
        });
    }

    ~Writer()
    {
        std::cout << "worker destructor before wait" << std::endl; //"queue size : " << m_logdata_queue.size() << std::endl;
        wait();
        std::cout <<
            "worker destructor after wait max queue size:  "
            << m_max_queue_size
            << " current queue size: " << m_logdata_queue.size()
            << std::endl; //"queue size : " << m_logdata_queue.size() << std::endl;
    }
    void cleanup()
    {

    }

    void submit(LogCallDataSPtr log_data_sptr)
    {
        {
            std::lock_guard<std::mutex> lock{m_queue_mutex};
            m_logdata_queue.push(log_data_sptr);
            m_queue_cond_var.notify_one();
        }
    }
    void loop()
    {
        m_max_queue_size = 0;
        for(;;) {
            std::this_thread::sleep_for (std::chrono::seconds());
            // std::cout << "Writer loop Q size: " << m_logdata_queue.size()  << std::endl;
            #if 1
            std::queue<LogCallDataSPtr>     tmp_logdata_queue;

            LogCallDataSPtr ldata_sptr;
            {
                std::unique_lock<std::mutex> lock{m_queue_mutex};
                m_queue_cond_var.wait(lock, [this]()
                {
                    return ((!m_logdata_queue.empty())&&(m_logdata_queue.size() > 0)) || (m_stop_flag);
                });
                m_max_queue_size = (m_max_queue_size > m_logdata_queue.size()) ? m_max_queue_size: m_logdata_queue.size();
                tmp_logdata_queue.swap(m_logdata_queue);
            }
            while(!tmp_logdata_queue.empty()) {
                ldata_sptr = tmp_logdata_queue.front();
                tmp_logdata_queue.pop();
                if (ldata_sptr->f != nullptr) {
                    ldata_sptr->f();
                }
                process(ldata_sptr);
            }
            if (m_stop_flag && m_logdata_queue.empty()) {
                // std::cout << "Writer stop flag is true" << std::endl;
                return;
            }
            #endif
        }
    }
    void wait()
    {
        LogCallDataSPtr d = std::make_shared<LogCallData>(
            __func__,
            LogLevelError,
            __FILE__,
            __LINE__,
            __func__,
            0,
            0
        );
        m_stop_flag = true;
        d->f = [this]()
        {
            std::cout << "from stop lambda" << std::endl;
        };
        d->message_sptr = std::make_shared<std::string>("writer is ending");
        submit(d);
        m_thread_object_uptr->join();
    }

    void process(LogCallDataSPtr ldata_sptr)
    {
        std::string output = m_formatter_ptr->format(ldata_sptr);
        m_sink_ptr->write_output(output);
    }
};
} // namespace
} // namespace

#endif