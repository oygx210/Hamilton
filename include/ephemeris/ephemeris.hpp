#pragma once

#include "math/vector3.hpp"

/**
 * Celestial object relative position and velocity
 */
struct EphemerisState
{
    // Position (m)    
    Vector3 Pos = Vector3::ZERO();

    // Velocity (m/s)    
    Vector3 Vel = Vector3::ZERO();

    // Light delay between reference object and target body (s)    
    double LightTime = 0.0;
};

/**
 * Base Ephemeris object
 */
class Ephemeris
{
public:
    Ephemeris() = default;

    // Should never copy
    Ephemeris(const Ephemeris& Eph) = delete;

    /**
     * Gets the current relative state of the tracked body in the 
     * default reference frame relative to the default reference object
     * @param EpochTime Time (s) in the reference epoch
     */
    EphemerisState GetState(double EpochTime) const;
};