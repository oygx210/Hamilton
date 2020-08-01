#pragma once

#include "math/vector3.hpp"
#include "math/spherical.hpp"

/* 
 * Interface for a gravitational model
 */
class GravityModel
{
public:
    virtual ~GravityModel() {}

    /* Acceleration (m/s2) in ENU co-ordinates */
    virtual Vector3 Acceleration(const Spherical& Sph, const TrigComponents& Trig) const noexcept = 0;
};