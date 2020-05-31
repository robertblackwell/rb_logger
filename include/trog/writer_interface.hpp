#ifndef trog_guard_worker_interface_hpp
#define trog_guard_worker_interface_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {

class WriterInterface
{
public:
    virtual void submit(LogCallDataSPtr log_data_sptr) = 0;
    virtual void cleanup() = 0;
    virtual void wait() = 0;
    virtual void process(LogCallDataSPtr ldata_sptr) = 0;

};

} // namespace

#endif