#pragma once

#include "disturbances/gravity.hpp"

enum class HarmonicOrder
{
    SPHERICAL, // Newtonian
    SECOND,    // Second order harmonics
    THIRD      // Third order harmonics
};

/** 
 * Gravitational model for the earth
 *
 * UNTESTED: 18 Jul 2020 
 */
template <HarmonicOrder Order = HarmonicOrder::SECOND>
class EarthGravity: public GravityModel
{
public:

    /** Update the cached value of gravitational acceleration */
    void Update(const Spherical& Sph, const TrigComponents& Trig);

private:
    // JGM3 Coefficients
    constexpr static double J2 = 1.75553E10;  // km^5 / s2
    constexpr static double J3 = -2.61913E11; // km^6 / s2
};