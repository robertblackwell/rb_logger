#ifndef RBLOGGER_config_HPP
#define RBLOGGER_config_HPP
//
/// configure the layout of the logger output line
#define RBLOG_USE_PREAMBLE
#undef RBLOG_FILENAME
#undef RBLOG_PIDTID
#define RBLOG_FUNCTION_NAME 1
#define RBLOG_LINE_NUMBER 1

// want to default to "ON" - disable Log by #define RBLOGGER_OFF
//#define RBLOGGER_OFF

///
/// These trun off/on tracing of special attributes
///

//#define NO_TRACE          // trace key points in the message traffic
//#define NO_CTORTRACE      // trace constructors and destructors
//#define NO_FDTRACE       // trace file descriptors
#define RBLOGGER_ON
#if !defined(RBLOGGER_OFF) || defined(RBLOGGER_ON) || defined(RBLOGGER_ENABLED)
    #define RBLOGGER_ENABLED
#else
    #undef RBLOGGER_ENABLED
#endif
#endif // header guard
