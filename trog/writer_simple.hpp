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

template <class F, class S>
class Writer: public F, public S
{
public:
    static std::shared_ptr<Writer<F, S>> make();
    Writer();
    ~Writer();
    void submit(LogCallDataSPtr log_data_sptr);
    void cleanup();
    void wait();
    void process(LogCallDataSPtr ldata_sptr);

private:
    std::mutex                      m_write_mutex;
    std::condition_variable         m_write_cond_var;
};


template<class F, class S>
Writer<F, S>::Writer()
{
}
template<class F, class S>
Writer<F, S>::~Writer()
{
}
template<class F, class S>
void Writer<F, S>::submit(LogCallDataSPtr log_data_sptr)
{
    process(log_data_sptr);
}
template<class F, class S>
void Writer<F, S>::cleanup()
{

}

template<class F, class S>
void Writer<F, S>::wait()
{
}
template<class F, class S>
void Writer<F, S>::process(LogCallDataSPtr ldata_sptr)
{
    // protect the writes from being interleaved with other writers
    std::lock_guard<std::mutex> lock{m_write_mutex};
    std::string output = F::format(ldata_sptr);
    S::write_output(output);
}

template<class F, class S>
std::shared_ptr<Writer<F, S>> Writer<F, S>::Writer::make()
{
    return std::make_shared<Writer>();
}


} //namespace
} // namespace

#endif