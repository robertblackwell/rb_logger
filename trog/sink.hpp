#ifndef trog_guard_generic_sink_hpp
#define trog_guard_generic_sink_hpp

#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>


namespace Trog {

class SinkInterface
{
public:
    virtual void write(std::string output) = 0;
};

typedef std::shared_ptr<SinkInterface> SinkInterfaceSPtr;
typedef std::vector<SinkInterfaceSPtr> SinkCollection;


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
    void write(std::string output)
    {
        std::cout << output << std::flush;
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
    void write(std::string output)
    {
        std::cerr << output << std::flush;
    }
};

class SinkFile;
using SinkFileSPtr = std::shared_ptr<SinkFile>;
class SinkFile: public SinkInterface
{
public:
    static SinkFileSPtr make(std::string filepath)
    {
        return std::make_shared<SinkFile>(filepath);
    }
    SinkFile(std::string filepath)
    {
        m_file.open(filepath);
    }
    void write(std::string output)
    {
        m_file << output << std::flush;
    }

private:
    std::string     m_filepath;
    std::ofstream   m_file;
};
} // namespace
#endif