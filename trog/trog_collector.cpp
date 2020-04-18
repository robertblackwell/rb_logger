
#include <trog/trog_collector.hpp>

#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <pthread.h>
#include <boost/filesystem.hpp>
#include <trog/trog_call_data.hpp>
#include <trog/trog_formatter.hpp>
#include <trog/trog_class.hpp>
#include <trog/worker.hpp>


namespace Trog {

std::map<std::string, CollectorSPtr> Collector::s_collectors;
    
CollectorSPtr Collector::get(std::string name)
{
    std::map<std::string, CollectorSPtr>::iterator x;
    if ((x = s_collectors.find(name)) == s_collectors.end()) {
        return nullptr;
    } else {
        return (*x).second;
    }
    return nullptr;
}
CollectorSPtr Collector::make(std::string name, WorkerSPtr worker_sptr, FormatterSPtr formatter_sptr)
{
    CollectorSPtr csptr = std::make_shared<Collector>(name, worker_sptr, formatter_sptr);
    Collector::s_collectors[name] = csptr;
    return csptr;
}

Collector::Collector(
        std::string name, 
        WorkerSPtr worker_sptr,
        FormatterSPtr formatter_sptr
    ): m_name(name), m_formatter_sptr(formatter_sptr), m_worker_sptr(worker_sptr)
    {

    }


} // namespace
