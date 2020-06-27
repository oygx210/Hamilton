// Load spice headers in platform independent way
extern "C"
{
    #if defined(__linux__)
        #include "linux/SpiceZpl.h"    
    #elif defined(__CYGWIN__)
        #include "cygwin/SpiceZpl.h"    
    #elif defined(_MSC_VER)
        #include "msvc/SpiceZpl.h"
    #elif defined(__APPLE__)
        #include "mac/SpiceZpl.h"
    #endif

    #include "SpiceZdf.h"   
    #include "SpiceErr.h"
    #include "SpiceEK.h"
    #include "SpiceFrm.h"
    #include "SpiceCel.h"
    #include "SpiceCK.h"
    #include "SpiceSPK.h"
    #include "SpiceGF.h"
    #include "SpiceOccult.h"
    #include "SpiceZpr.h"
    #include "SpiceZim.h"
}

#include <string>

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
    #define DEFAULT_LEAP_SECOND_KERNAL (std::string(PROJECT_DIR) + std::string("/data/spice/leap_seconds_kernel/naif0012.tls.pc"))
#else
    #define DEFAULT_LEAP_SECOND_KERNAL (std::string(PROJECT_DIR) + std::string("/data/spice/leap_seconds_kernel/naif0012.tls"))
#endif