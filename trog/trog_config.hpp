#ifndef trog_guard_config_HPP
#define trog_guard_config_HPP
//
/// configure the layout of the logger output line
#define RBLOG_USE_PREAMBLE
#undef RBLOG_FILENAME
#undef RBLOG_PIDTID
#define RBLOG_FUNCTION_NAME 1
#define RBLOG_LINE_NUMBER 1

// want to default to "ON" - disable Log by #define Trog_OFF
//#define Trog_OFF

///
/// These trun off/on tracing of special attributes
///

//#define NO_TRACE          // trace key points in the message traffic
//#define NO_CTORTRACE      // trace constructors and destructors
//#define NO_FDTRACE       // trace file descriptors
#define Trog_ON
#if !defined(Trog_OFF) || defined(Trog_ON) || defined(Trog_ENABLED)
    #define Trog_ENABLED
#else
    #undef Trog_ENABLED
#endif

#endif // header guard
