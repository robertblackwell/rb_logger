#ifndef trog_guard_generic_sink_hpp
#define trog_guard_generic_sink_hpp

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <trog/filesystem.hpp>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace fs;

namespace Trog {

/** A simple naming policy for log files*/
class SinkFileName
{
public:
    std::string makeFileName()
    {
        path cwd = current_path();
        path result = cwd / ("trog_" + timestamp() + ".log");
        return result.string();
    }
private:
    std::string timestamp()
    {
        auto time = std::time(nullptr);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%F_%T"); // ISO 8601 without timezone information.
        auto s = ss.str();
        std::replace(s.begin(), s.end(), ':', '-');
        return s;
    }
};

class SinkInterface
{
public:
    virtual void write_output(std::string output) = 0;
};

typedef std::shared_ptr<SinkInterface> SinkInterfaceSPtr;


class Sinkstdout;

using SinkstdoutSPtr = std::shared_ptr<Sinkstdout>;

class Sinkstdout: public SinkInterface
{
public:
    static SinkstdoutSPtr New()
    {
        return std::make_shared<Sinkstdout>();
    }
    static std::shared_ptr<Sinkstdout> make()
    {
        return std::make_shared<Sinkstdout>();
    }
    void write_output(std::string output)
    {
        std::cout << output  << std::endl <<std::flush;
    }
};

class Sinkstderr;
typedef std::shared_ptr<Sinkstderr> SinkstderrSPtr;

class Sinkstderr: public SinkInterface
{
public:
    static SinkstderrSPtr make()
    {
        return std::make_shared<Sinkstderr>();
    }
    void write_output(std::string output)
    {
        std::cerr << output << std::flush;
    }
};

template <typename N>
class SinkFileT: public N, public SinkInterface
{
public:
    static std::shared_ptr<SinkFileT> make()
    {
        return std::make_shared<SinkFileT<N> >();
    }
    SinkFileT()
    {
        m_file_name = N::makeFileName();
        m_file.open(m_file_name);
    }
    void write_output(std::string output)
    {
        m_file << output << std::endl << std::flush;
    }

private:
    std::ofstream   m_file;
    std::string     m_file_name;
};
// convenince type name
typedef SinkFileT<SinkFileName> SinkFile;

template<typename S1>
class Sink1: public SinkInterface
{
    S1 m_s1;
    public:
    static std::shared_ptr<Sink1> make()
    {
        return std::make_shared<Sink1<S1> >();
    }
    void write_output(std::string output)
    {
        m_s1.write_output(output);
    }
};

template<typename S1, typename S2>
class Sink2: public SinkInterface
{
    S1 m_s1;
    S2 m_s2;
    public:
    static std::shared_ptr<Sink2> make()
    {
        return std::make_shared<Sink2<S1, S2> >();
    }
    void write_output(std::string output)
    {
        m_s1.write_output(output);
        m_s2.write_output(output);
    }
};

// convenience typename
typedef Sink2<Sinkstdout, SinkFile> SinkDefault;

} // namespace
#endif