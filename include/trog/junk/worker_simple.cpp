#include <trog/worker_simple.hpp>

#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <trog/formatter.hpp>
#include <trog/call_data.hpp>
#include <trog/sink.hpp>

namespace Trog {
namespace Simple {

Writer::Writer(FormatterSPtr formatter_sptr, SinkCollection& sinks): m_sinks(sinks)
{
    m_formatter_sptr = formatter_sptr;
}
Writer::~Writer()
{
}
void Writer::submit(LogCallDataSPtr log_data_sptr)
{
    process(log_data_sptr);
}
void Writer::cleanup()
{

}

void Writer::wait()
{
}
void Writer::process(LogCallDataSPtr ldata_sptr)
{
    std::string output = m_formatter_sptr->format(ldata_sptr);
    for(int index = 0; index < m_sinks.size(); index++) {
        m_sinks[index]->write(output);
    }
}

WriterSPtr Writer::Writer::make(FormatterSPtr formatter_sptr, SinkCollection& sinks)
{
    return std::make_shared<Writer>(formatter_sptr, sinks);
}

} //namespace
} // namespace
