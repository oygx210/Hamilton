#pragma once

#include "disturbances/gravity.hpp"

#include "math/constants.hpp"
#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "math/spherical.hpp"

enum class HarmonicOrder
{
    SPHERICAL, // Newtonian
    SECOND,    // Second order harmonics 
    THIRD      // Third order harmonics
};

/* 
 * Gravitational model for the earth
 * 
 * Uses the dominant terms modal harmonic terms for an approximate and efficient earth gravitation model
 * 
 */
template <HarmonicOrder Order> 
class OblateEarthGravity: virtual public GravityModel
{
public:

    OblateEarthGravity() {}

    /*
     * Static calculate acceleration
     */
    constexpr static Vector3 CalculateAcceleration(const Spherical& Sph, const TrigComponents& Trig) noexcept
    {
        // Newtonian contribution    
        double AccelerationRadial = -EARTH::GRAVITATIONAL_CONSTANT / (Sph.Rad * Sph.Rad);
        double AccelerationInclined = 0.0;

        // Higher order contributions
        if constexpr (Order != HarmonicOrder::SPHERICAL)
        {
            // Calculate radius^4 (km^4)
            double Radius = Sph.Rad * 1.0E-3;
            double RadiusQuart = Radius;
            RadiusQuart *= RadiusQuart;
            RadiusQuart *= RadiusQuart;
            double SinInc2 = Trig.SinInc * Trig.SinInc;
        
            // Second order contribution    
            AccelerationInclined -= J2 * 3.0 * Trig.CosInc * Trig.SinInc / RadiusQuart * 1.0E3;
            AccelerationRadial += J2 * 1.5 * (3.0 * SinInc2 - 1.0) / RadiusQuart * 1.0E3;

            // Third order contribution
            if constexpr (Order == HarmonicOrder::THIRD)
            {
                double RadiusPent = RadiusQuart * Radius;
                AccelerationInclined -= J3 * 1.5 * Trig.CosInc * (5.0 * SinInc2 - 1.0) / RadiusPent * 1.0E3;
                AccelerationRadial += J3 * 2.0 * (5.0 * SinInc2 - 3.0) / RadiusPent * 1.0E3;
            }        
        }

        return Vector3({0.0, AccelerationInclined, AccelerationRadial});
    }

    Vector3 Acceleration(const Spherical& Sph, const TrigComponents& Trig) const noexcept
    {
        return OblateEarthGravity::CalculateAcceleration(Sph, Trig);
    }    

private:
    // JGM3 Coefficients
    constexpr static double J2 = 1.75553E10;  // km^5 / s2
    constexpr static double J3 = -2.61913E11; // km^6 / s2
};