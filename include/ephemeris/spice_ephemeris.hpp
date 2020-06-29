#pragma once

#include <string>
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
    SpiceEphemeris(const std::string& Object, 
                   const std::string& Frame, 
                   const std::string& Reference);

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
    std::string mObjectID; // Spice object ID
    std::string mFrame;          // Reference object frame ID
    std::string mReference;      // Reference object Spice ID
};