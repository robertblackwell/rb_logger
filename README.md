# trog

__trog__ is a personal c++ header only logging library.

## Background

For a number of years I have been using __trog__, a logging system I hacked up from something I got in the obj-c community; its been so long I can't even remember now where it came from.

Recently a current project required me to add some features and fix a few things. This got me researching available c++ logging system. I was intrigued to read the discussion of latency between __spdlog__, __reckless__ and __nanolog__, and as a result I did a few timing experiments related to formating log output lines and concluded this was an area where I did not want to spend a lot of effort. These latency discussions also caused me to research lockless queues and other inter-thead and inter-process mechanisms as vehicles for communicating log data from the site where it is generated to the site where it is written, and generally thinking about the structure of a logging system and the various trade-offs. Ultimately these considerations along with a desire expand my knowledge of c++ templates (and particularly the concept of Policy templates) on a small project resulted in me commencing a complete rewrite of __trog__.

The aim of the rewrite is not latency performance, my needs are sufficiently pedestrian that latency is (I think) not an issue. I am more interested in using the rewrite as vehicle for exploring different backend arrangements:

1. a backend for a single thread application where the log writer runs on the main thread,
2. a multi-threaded application where the writer runs on whatever thread generates the log data and "locks" the output mechanism (console/file) to ensure log output is not corrupted.

3. a multi-threaded application where the writer runs in a dedicated "backend" thread.

   This structure seems to give more possibility of loosing log records. So far my research suggests that under exception circumstances (such as divide by zero) there is no way of ensuring that the writer writes and flushes all output before the program aborts. Of course one could always make the code that generates the log data wait for the writer to complete, but then there is no benefit to having a backend thread.

4.  An intriguing idea is to have a backend/writer process rather than a writer thread. That way the writer (and the queue mechanism) can persisit even if the application aborts and ensure that all log data that got to the queue mechaism will get written. If I get the time to try this I will almost certainly use IPC message queues (see boost) as the queue mechanism. Shared memory (again see boost) would probably give superior performance but seems like a long learning curve. 

~~Finally I am interested in using this project as a vehicle for leaning about templates, Policy classes and how to use template classes as an alternative to abstract inheritence.~~

At one point (V2.0.0) this project used the concepts of policy templates to provide the mechanism for choosing Sink, Format and Worker thread mechanism. However I eventually discovered that for one of my library projects I needed to be able to make different choices for different executables without recompiling the library. This forced me to make such choices at run-time and hence to abandon compile time polymorphism for more traditional inheritence.

Interesting note:

https://stackoverflow.com/questions/43540943/using-boost-lockfree-queue-is-slower-than-using-mutexes

## First Pre-release

Since writing the first draft of this readme I have implemented much of what I planned so that now __trog__ is usable (at least by me) and I have some initial performance numbers, see below.

Features so far implemented:

#### Macros

An application creates a log message by using a macros such as:

```TROG_DEBUG("a string message", an_int, a_double, a_string_variable)```

there are a suite of these for various levels and purposes.

#### Sink

A sink is a class that represents where the log is written such as stdout, stderr or a file.

Trog provides sinks for stdout, stderr, a file (whose name is of the form trog_yyyy-mm-dd_hh-mm-ss.log), and any two of these combined.

#### Writer

 A writer is a class that is passed data from the TROG_XXXXX macros and arranges to pass it to the currently active sink.

Currently there are two types of writers:

__Simple::Writer__ -  it runs on the same thread as the application that called the TROG_XXXX macro that generated the log messsage. Since the application using Trog may be multi-threaded, this type of  writer may have to operate on multiple threads. It uses a lock (mutex/condition variable) to ensure that logs generated on different threads do not conflict.

__Thread::Writer__    is a writer that runs on a dedicated "background" thread. The TROG_XXXX macros call functions that pass log data from the TROG_XXXX macros to the background thread via a queue mechanism. The background thread  completes the formatting of the output message and writes to the sink.

## Prerequisites

Current development work is undertaken in C++ (14/17) on Ubuntu 18.04 with g++ 9.0, CMake 3.17, vscode 1.44.2,  the boost::filesystem library 1.72.1, and doctest - so thats where, currently (April 2020), the package is known to work.

## Installation

In the event that the reader is interested in trying it, the project can installed by cloning this repo.

To install dependencies boost and doctest in the projects __vendor__ directory run the following command from the project root directory:

```bash
./scripts/install_dependencies.sh install
```
to build the tests and sample app, the performance test and the unit tests run:

```make```

That will perform a cmake build in cmke-build-debug and deposit a debug version of the sample app in

```cmake-build-debug/sample-app/sample```

If you want to use __trog__ in a project of your own you will need to get the trog header files into your "include" directory, or at least an "include" directory your project accesses. Run the following command from the trog repo root dirctory:

```make install install-prefix=<your_directory> ```

(note the lower case install-prefix)this will depost the tro headers into __your_directory/include/trog__. Remember trog will require boost::filesystem.

## Usage

see the sample application

## Sample application

There is a sample application in trog/sample-app within the repo. It is a good place to go to learn how to use __trog__.

Please make sure to update tests as appropriate.

## Performance

Two simple speed test apps are available in __performance/dedicated_thread_speed.cpp__, and __performance/user_thread_speed.cpp__.

Each app was run on the follwing test.

The test issued 1,000,000 __TROG_ERROR("some message")__ requests in a tight loop (no delays or other activity). The total elapsed time was measured using `std::chrono::high_resolution_clock`in __nano seconds__.

The results:

#### Simple::Writer

- 4,750,500,800 total nano-secs
- 4,750 nano seconds per TROG_ERROR invocation.

#### Thread::Writer

- 1,400,600,700 nano seconds total
- 1,400 nano secs per TROG_ERROR invocation.

It is interesting to note that in the Thread::Writer tests the backend queue grew very large with as many as 500,000 entries being the max and that when the test loop ended there were still around 300,000 requests in the queue not yet written. The Thread::Writer was able to remain operating even after the application main()  function exited and write all the messages before being destructed.

The bad news for the Thread::Writer is the following:

- the strategy for keeping the thread writer functioning after the main( ) function exists (and hence being able to flush the queue) has the follwing elements:
  - make the writer a static singleton so that it's constructor is called before main() executes and its destructor  is called after main returns. This covers the situation of a normal program termination.
  - is uses __std::set_terminate()__  to run a function (which flushes the queue) after an uncaught C++ exception terminates the main() function.

- However these strategies do not work when a program aborts due to errors which are not C++ exceptions such as illegal pointer access, arithmetic overflow and divide by zero.

Thus there is a tradeoff (not surprisingly)

- if latency is important use Thread::Writer and get log calls down to __1.5 micro seconds__. -- but risk loosing log messages under some failure scenarious.
- on the other hand if you cannot accept loosing log messages use Simple::Writer and have to accept log calls costing you __5 micro seconds__.

In the interest of full disclosure these tests where run on Ubuntu 18.04, AMD Ryzen 3900x, 64GB memory 1T Intel M2 drive.

Since I am still in dev mode the code was compiled with the -g option.

Some time soon I will do the tests on some of my old Macbooks, that should be interesting.

## License

None
