#pragma once

#include "math/core_math.hpp"

/* 
 * Simple container for a spherical (radius, azimuth, inclination) 
 * co-ordinate
 * Inclination is measured relative to the x-y plane, i.e an
 *   inclination of +- PI / 2 gives the poles and 0 gives the equator
 *   Azimuth is measured counterclockwise from the + x axis
 */
struct Spherical
{
    double Rad = 0.0; // radius (m)
    double Azm = 0.0; // azimuth (rad)
    double Inc = 0.0; // inclination (rad)
};

/** 
 * Container for trigonometric components
 */
struct TrigComponents
{
    double SinAzm = 0.0;
    double CosAzm = 0.0;
    double SinInc = 0.0;
    double CosInc = 0.0;
};

/*  
 * Helper function to evaluate the sin and cos components of a spherical 
 * co-ordinate
 */
inline constexpr TrigComponents CalculateTrigComponents(const Spherical& Sph)
{
    return TrigComponents{
        .SinAzm = Sin(Sph.Azm),
        .CosAzm = Cos(Sph.Azm),
        .SinInc = Sin(Sph.Inc),
        .CosInc = Cos(Sph.Inc)
    };
}