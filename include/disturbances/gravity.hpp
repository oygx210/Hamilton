#pragma once

#include "math/vector3.hpp"
#include "math/spherical.hpp"

/* 
 * Base class for a gravitational model
 */
class GravityModel
{
public:
    virtual ~GravityModel() {}

    /* Acceleration (m/s2) in ENU co-ordinates */
    virtual static Vector3 CalculateAcceleration(const Spherical& Sph, const TrigComponents& Trig) noexcept = 0;
};