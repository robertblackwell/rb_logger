#ifndef trog_guard_formatter_HPP
#define trog_guard_formatter_HPP
//
#include <iostream>
#include <sstream>
#include <set>
#include <cstdint>
#include <pthread.h>
#include <boost/filesystem.hpp>
#include <trog/call_data.hpp>
#include <trog/loglevel.hpp>

namespace Trog {

class FormatterInterface
{
    virtual std::string format(LogCallDataSPtr data_sptr) = 0;
};

class Formatter;
using FormatterSPtr = std::shared_ptr<Formatter>;


/** Object that formats the message part of a LogCallData object during collections
 * and formats the entire logger output record in the backend.*/
class Formatter: public FormatterInterface
{
    public:

    static FormatterSPtr make();

    // void format_types(std::ostringstream& os);
    template <typename T, typename... Types>
    void format_types (std::ostringstream& os,  const T& firstArg)
    {
        os << " " << firstArg ;
        // format_types(os);
    }

    template <typename T, typename... Types>
    void format_types (std::ostringstream& os,  const T& firstArg, const Types&... args)
    {
        os << " " << firstArg ;
        format_types(os, args...);
    }

    void preamble(
        std::ostringstream& os,
        LogLevelType level,
        std::string filename,
        long pid,
        long tid,
        std::string function_name,
        long linenumber
    ){
        boost::filesystem::path p(filename);
        std::string s = p.filename().string();
        std::string ls = LogLevelText(level);
        os 
            << ls << "|"
            #ifndef RBLOG_FILENAME
            << s  
            #endif
            #ifndef RBLOG_PIDTID
            << "[" 
            <<pid 
            << ":" 
            << tid 
            <<"]" 
            #endif
            #ifndef RBLOG_FUNCTION_NAME
            << "::"
            << function_name 
            #endif
            #ifndef RBLOG_LINE_NUMBER
            << "["<< linenumber <<"]"
            << ":" 
            #endif
            << "";
    }

    template <typename T, typename... Types>
    void fmt_log(std::ostringstream& os, char* file_name, long pid, long tid, char* function_name, int line_number, const T& firstArg, const Types&... args)
    {
        format_types(os, firstArg, args...);
    }
    std::string format(LogCallDataSPtr log_data_sptr)
    {
        std::ostringstream os;
        LogCallDataSPtr sp = log_data_sptr;
        preamble(os,
            sp->level,
            sp->file_name,
            sp->pid,
            sp->tid,
            sp->function_name,
            sp->line_number
        );
        os << *log_data_sptr->message_sptr;
        return os.str();
    }
};

inline FormatterSPtr Formatter::make()
{
    return std::make_shared<Formatter>();
}

} // namespace
#endif