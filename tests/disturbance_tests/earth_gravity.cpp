#include "disturbances/earth_gravity.hpp"
#include "coordinates/earth.hpp"
#include "gtest/gtest.h"

#include <memory> 

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
        printf("Acceleration (m/s2) = %s\n", Accel.ToString().Data());

        constexpr auto Accel2ndOrder = OblateEarthGravity<HarmonicOrder::SECOND>::CalculateAcceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", Accel2ndOrder.ToString().Data());

        constexpr auto Accel3rdOrder = OblateEarthGravity<HarmonicOrder::THIRD>::CalculateAcceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", Accel3rdOrder.ToString().Data());
    }

    // Calculate acceleration at this point (runtime context)
    {
        std::unique_ptr<GravityModel> GravModel = std::make_unique<OblateEarthGravity<HarmonicOrder::SPHERICAL>>();    
        auto Accel = GravModel->Acceleration(Point, Trig);
        printf("Acceleration (m/s2) = %s\n", Accel.ToString().Data());
    }        
}