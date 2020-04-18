#ifndef trog_guard_collector_HPP
#define trog_guard_collector_HPP
//
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

class Collector;

using CollectorSPtr = std::shared_ptr<Collector>;

/** a class the collects logging data at the site of a call toi the logger system.
 * there can be multiple of these objects.
 * Each one is distinguised by a 'name'
*/
class Collector
{
public:
    static std::map<std::string, CollectorSPtr> s_collectors;
    static CollectorSPtr get(std::string name);
    static CollectorSPtr make(std::string name, WorkerSPtr worker_sptr, FormatterSPtr formatter_sptr);

    Collector(
        std::string name, 
        WorkerSPtr worker_sptr,
        FormatterSPtr formatter_sptr
    );
    
    template<typename T, typename... Types>
    void collect(
        LogLevelType level,
        LogLevelType threshold,
        const char* channel,
        const char* file_name,
        const char* func_name,
        int line_number,
        const T& firstArg,
        const Types&... args)
    {
        LogCallDataSPtr call_sptr = std::make_shared<LogCallData>(
            channel, 
            level, 
            file_name, 
            line_number, 
            func_name, 
            pthread_self(), 
            getpid()
        );
        // tests suggest something like the following would save about 1/3
        // compared to the ostringstream version in the else clause
        #if 0
        char dummy[] = "This is a dummy string"; 
        char* p = (char*)malloc(1000);
        char* rp = p;
        for(int i = 0; i < 10; i++) {
            int n = sprintf(rp, "%s", dummy);
            rp = rp+n;
        }
        #else
        std::ostringstream os;
        m_formatter_sptr->format_types(os, firstArg, args...);
        call_sptr->message_sptr = std::make_shared<std::string>(os.str());
        m_worker_sptr->addToQueue(call_sptr);
        #endif
    }
private:
    std::string     m_name;
    FormatterSPtr   m_formatter_sptr;
    WorkerSPtr      m_worker_sptr;
}; //class   

} // namespace
#endif