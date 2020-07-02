#pragma once

#include "ephemeris/ephemeris.hpp"
#include "spice_ephemeris.hpp"

#include <string>

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
    #define DEFAULT_LEAP_SECOND_KERNAL (std::string(PROJECT_DIR) + std::string("/data/spice/leap_seconds_kernel/naif0012.tls.pc"))
#else
    #define DEFAULT_LEAP_SECOND_KERNAL (std::string(PROJECT_DIR) + std::string("/data/spice/leap_seconds_kernel/naif0012.tls"))
#endif

/* 
 * Ephemeris object using JPL NAIF's Spice as the backend
 */
class SpiceEphemeris: public Ephemeris
{
public:
    SpiceEphemeris() = default;
    ~SpiceEphemeris() = default;

    // Disable copy
    SpiceEphemeris(const SpiceEphemeris& Copy) = delete;

    // Standard initialiser
    SpiceEphemeris(const Spice::EphemerisInputs& Inputs) noexcept;

    /* 
     * Gets the current relative state of the tracked body in the 
     * default reference frame relative to the default reference object
     */
    EphemerisState GetState(double EpochTime) const noexcept;

private:

    Spice::EphemerisInputs mParams{};
};