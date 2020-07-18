#pragma once

#include "math/vector3.hpp"
#include "math/spherical.hpp"

/** 
 * Base class for a gravitational model
 */
class GravityModel
{
public:
    /** Get acceleration (m/s2) */
    constexpr const Vector3& Acceleration(void) const noexcept {return mAcceleration;}

    /** Update the cached value of gravitational acceleration */
    virtual void Update(const Spherical& Sph, const TrigComponents& Trig) = 0;

private:

    // (m/s2)
    Vector3 mAcceleration;
    
};