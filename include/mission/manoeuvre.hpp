#pragma once

#include "math/core_math.hpp"

// 
// HACK HACK HACK!: This code does not meet quality standards and needs review
//

// Output of a launch to inclination/velocity maneouvre
struct LaunchVelocityResult
{ 
    double Vx = 0.0;              // X component of delta v required (m/s)
    double Vy = 0.0;              // Y component of delta v required (m/s)
    double AzimuthInertial = 0.0; // Azimith to launch to if the source body was not rotating
    double Azimuth = 0.0;         // True Azimuth to launch to (rad)
};

// Inputs to a launch to inclination/velocity maneouvre
struct LaunchVelocityInputs
{
    double TargetInclination = 0.0;  // Required orbital inclination (rad)
    double SiteLatitude = 0.0;       // Launch site latitude (rad)
    double OrbitalVelocity = 0.0;    // Required orbital velocity (rad)
    double SiteVelocity = 0.0;       // Linear velocity due to body rotation at the launch site (m/s)
}; 

/* 
 * Rough calculation of some basic launch parameters to acehive a given (circular) orbital
 * velocity and inclination from a launch site latitude and velocty
 */
constexpr LaunchVelocityResult LaunchVelocityComponents(const LaunchVelocityInputs& Inputs)
{
    LaunchVelocityResult Result;

    if (Inputs.SiteLatitude > Inputs.TargetInclination)
    {
        printf("Cannot acheive desired inclination %g, from launch site at latitude %g in a single manoeuvre\n",
            R2D(Inputs.TargetInclination), R2D(Inputs.SiteLatitude));
        return Result;
    }

    auto CosLatitude = Cos(Inputs.SiteLatitude);
    if (CosLatitude == 0)
    {
        // We are at the north or south pole, the concept of azimuth breaks down here
        Result.AzimuthInertial = 0.5 * PI;
    }
    else
    {
        Result.AzimuthInertial = Asin(Cos(Inputs.TargetInclination) / CosLatitude);
    }

    Result.Vx = Inputs.OrbitalVelocity * Sin(Result.AzimuthInertial) - Inputs.SiteVelocity * CosLatitude;
    Result.Vy = Inputs.OrbitalVelocity * Cos(Result.AzimuthInertial);
    Result.Azimuth = Atan2(Result.Vx, Result.Vy);

    return Result;    
}