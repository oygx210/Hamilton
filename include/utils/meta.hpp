#pragma once

/**
 * @file meta.hpp
 * Platform independent macros to selectively ignore a compiler warning 
 * without globally unsetting the flag. 
 *
 * Should only be used when a correct fix cannot be avoided, i.e when interfacing 
 * with external codebases which contain the given warnings
 */

#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH           __pragma(warning( push ))
    #define DISABLE_WARNING_POP            __pragma(warning( pop )) 
    #define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))
 
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER         DISABLE_WARNING(4100)
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION                 DISABLE_WARNING(4505)
    #define DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA DISABLE_WARNING(4244)
    // add additional warnings here
    
#elif defined(__CYGWIN__) || defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop) 
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)
    
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER         DISABLE_WARNING(-Wunused-parameter)
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION                 DISABLE_WARNING(-Wunused-function)
    #define DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA DISABLE_WARNING(-Wconversion)
   // add additional warnings here 
    
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
    #define DISABLE_WARNING_UNREFERENCED_FUNCTION
    #define DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA
    // add additional warnings here
 
#endif
    
