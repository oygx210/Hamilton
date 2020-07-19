#include "disturbances/earth_gravity.hpp"
#include "coordinates/earth.hpp"
#include "gtest/gtest.h"

// TODO: Complete
TEST(Gravity, Earth)
{
    // Make a point on the earths equator
    constexpr auto Radii = Earth::WGS84RadiiComponents(0.0);
    constexpr auto Point = Cart2Sph(Earth::LLA2ECEF(LLA{0.0, 65.0, 0.0}, Radii));
    constexpr auto Trig = CalculateTrigComponents(Point);

    // Calculate acceleration at this point (constexpr context)
    {
        constexpr auto Accel = OblateEarthGravity<HarmonicOrder::SPHERICAL>::CalculateAcceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", Accel.ToString().c_str());

        constexpr auto Accel2ndOrder = OblateEarthGravity<HarmonicOrder::SECOND>::CalculateAcceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", Accel2ndOrder.ToString().c_str()); 

        constexpr auto Accel3rdOrder = OblateEarthGravity<HarmonicOrder::THIRD>::CalculateAcceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", AccelThirdOrder.ToString().c_str());                 
    }

    // Calculate acceleration at this point (runtime context)
    {
        auto GravModel = OblateEarthGravity<HarmonicOrder::SPHERICAL>();    
        auto Accel = GravModel.CalculateAcceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", Accel.ToString().c_str());
    }        
}