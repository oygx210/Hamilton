
#include "math/core_math.hpp"
#include "mission/manoeuvre.hpp"
#include "math/constants.hpp"
#include "coordinates/earth.hpp"
#include "gtest/gtest.h"

TEST(LaunchManoeuvre, CapeCanaveral)
{
    {    
        auto Latitude = D2R(28.5);
        auto Inclination = D2R(51.6);      // International Space Station Inclination
        auto OrbitalVelocity = 7730.0;     // m/s, for a 300 km circular orbit
        auto LaunchSiteVelocity = 465.101; // m/s

        auto LaunchComponents = LaunchVelocityComponents({
            .TargetInclination = Inclination,
            .SiteLatitude = Latitude,
            .OrbitalVelocity = OrbitalVelocity,
            .SiteVelocity = LaunchSiteVelocity});

        printf("AzimuthTrue = %g, AzimuthInertial = %g, V = (%g, %g)\n", 
            R2D(LaunchComponents.Azimuth), 
            R2D(LaunchComponents.AzimuthInertial),
            LaunchComponents.Vx,
            LaunchComponents.Vy);
    }
}

TEST(LaunchManoeuvre, AbbottsPoint)
{
    {
        auto Latitude = D2R(-19.0);
        auto Inclination = D2R(98.0);
        auto OrbitalVelocity = Sqrt(Earth::GRAVITATIONAL_CONSTANT / (Earth::WGS84::SEMI_MAJOR_AXIS + 500.0E3)); // m/s
        
        auto LaunchSiteVelocity = Earth::ROTATIONAL_RATE * Earth::WGS84Radius(D2R(-19.0)) * Cos(Latitude); // m/s

        auto LaunchComponents = LaunchVelocityComponents({
            .TargetInclination = Inclination,
            .SiteLatitude = Latitude,
            .OrbitalVelocity = OrbitalVelocity,
            .SiteVelocity = LaunchSiteVelocity});

        printf("Orbital Velocity = %g, AzimuthTrue = %g, Azimuth Inertial = %g, V = (%g, %g)\n",
            OrbitalVelocity, 
            R2D(LaunchComponents.Azimuth), 
            R2D(LaunchComponents.AzimuthInertial),
            LaunchComponents.Vx,
            LaunchComponents.Vy);

        printf("Launch Site Velocity = %g m/s\n", LaunchSiteVelocity);
        printf("Latitude Delta V Penalty = %g m/s\n", 
            Sqrt(LaunchComponents.Vx * LaunchComponents.Vx + LaunchComponents.Vy * LaunchComponents.Vy) - OrbitalVelocity);
    }    
}