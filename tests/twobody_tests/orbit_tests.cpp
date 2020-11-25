#include "gtest/gtest.h"

#include "twobody/orbit.hpp"

TEST(Orbit, CircularEquatorialOrbit)
{
    double Radius = Earth::WGS84::SEMI_MAJOR_AXIS + 400E3;    
    TwoBody::KeplerianElements Elements{.SemiParameter = Radius, .SemiMajorAxis = Radius, .GravitationalParameter = Earth::GRAVITATIONAL_CONSTANT};

    auto Orbit = TwoBody::Orbit::FromKeplerianElements(Elements);

    // Check orbit is as expected
    const auto Period = TwoBody::CalculatePeriod(Elements);

    ASSERT_EQ(Orbit.GetClassification(), TwoBody::OrbitClassification::CIRCULAR_EQUATORIAL);
    ASSERT_EQ(Orbit.GetRadius(), Radius);
    ASSERT_EQ(Orbit.GetPeriod(), Period);
    ASSERT_EQ(Orbit.GetElements().TrueLongitude, 0.0);

    // Advance time by 1/4'th the period    
    Orbit.Update(Orbit.GetPeriod() * 0.25);
    ASSERT_EQ(Orbit.GetClassification(), TwoBody::OrbitClassification::CIRCULAR_EQUATORIAL);
    ASSERT_EQ(Orbit.GetRadius(), Radius);
    ASSERT_EQ(Orbit.GetPeriod(), Period);
    ASSERT_EQ(Orbit.GetElements().TrueLongitude, PI * 0.5);
    
    // Advance time by 1/2 the period    
    Orbit.Update(Orbit.GetPeriod() * 0.5);
    ASSERT_EQ(Orbit.GetClassification(), TwoBody::OrbitClassification::CIRCULAR_EQUATORIAL);
    ASSERT_EQ(Orbit.GetRadius(), Radius);
    ASSERT_EQ(Orbit.GetPeriod(), Period);
    ASSERT_EQ(Orbit.GetElements().TrueLongitude, 1.5 * PI);

    // Advance time by 6 periods
    Orbit.Update(Orbit.GetPeriod() * 6);
    ASSERT_EQ(Orbit.GetClassification(), TwoBody::OrbitClassification::CIRCULAR_EQUATORIAL);
    ASSERT_EQ(Orbit.GetRadius(), Radius);
    ASSERT_EQ(Orbit.GetPeriod(), Period);
    ASSERT_EQ(Orbit.GetElements().TrueLongitude, 1.5 * PI);    
        
}