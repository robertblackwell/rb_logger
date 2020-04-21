#ifndef trog_guard_config_HPP
#define trog_guard_config_HPP
//
#define Trog_ON
#if !defined(Trog_OFF) || defined(Trog_ON) || defined(Trog_ENABLED)
    #define Trog_ENABLED
#else
    #undef Trog_ENABLED
#endif

#endif // header guard
