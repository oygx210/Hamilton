#pragma once

#include <string_view>
#include <vector>
#include <cstdio>
#include "ephemeris.hpp"
#include "spice_imports.hpp"

/* 
 * Interface to the spice ephemeris subroutines, provides a modern interface to the underlying
 * JPL functionality
 */
class SpiceEphemeris: public Ephemeris
{
public:

    // Initialise using spice object ID, default frame ID and default relative object
    SpiceEphemeris(const std::string_view& Object, 
                   const std::string_view& Frame, 
                   const std::string_view& Reference);

    // Default constructor
    SpiceEphemeris();

    // Default destructor
    ~SpiceEphemeris() = default;    

    /* 
     * Gets the current relative state of the tracked body in the 
     * default reference frame relative to the default reference object
     */
    EphemerisState GetState(double EpochTime) const;

private:
    std::string_view mObjectID; // Spice object ID
    std::string_view mFrame;          // Reference object frame ID
    std::string_view mReference;      // Reference object Spice ID
};