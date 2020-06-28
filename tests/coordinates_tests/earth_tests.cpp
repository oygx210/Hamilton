#include "gtest/gtest.h"
#include "test_utils.hpp"
#include "math/constants.hpp"
#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "coordinates/earth.hpp"

TEST(Earth, WGS84)
{
    // Equatorial Radius
    static_assert(Earth::WGS84Radius(0.0) == EARTH::WGS84::SEMI_MAJOR_AXIS);

    // Polar Radius
    static_assert(Earth::WGS84Radius(PI * 0.5) == EARTH::WGS84::SEMI_MINOR_AXIS);    
    static_assert(Earth::WGS84Radius(-PI * 0.5) == EARTH::WGS84::SEMI_MINOR_AXIS);      

    // Radius at a latitude of +-55.0 deg    
    static_assert(IsNear(Earth::WGS84Radius(D2R(55.0)), 6363827.355717048, 1.0E-4));
    static_assert(IsNear(Earth::WGS84Radius(D2R(-55.0)), 6363827.355717048, 1.0E-4));

    // Check radial components for ECEF <-> LLA calculation
    constexpr auto EquatorialComponents = Earth::WGS84RadiiComponents(0.0);
    constexpr auto PolarComponents = Earth::WGS84RadiiComponents(PI * 0.5);

    // Check calculation of equatorial components
    constexpr auto Curvature = 1.0 - EARTH::WGS84::ECCSQ;    
    {
        static_assert(EquatorialComponents.Azimuthal == EARTH::WGS84::SEMI_MAJOR_AXIS);
        static_assert(IsNear(EquatorialComponents.Inclined, EARTH::WGS84::SEMI_MAJOR_AXIS * Curvature, 1.0E-4));
    }

    // Check calculation of polar components
    {
        static_assert(IsNear(PolarComponents.Azimuthal, EARTH::WGS84::SEMI_MAJOR_AXIS / Sqrt(Curvature), 1.0E-4));
        static_assert(IsNear(PolarComponents.Inclined, EARTH::WGS84::SEMI_MINOR_AXIS, 1.0E-4));
    }    

}


// Inertial to body fixed frame
TEST(Earth, QuatECI2ECEF)
{
    constexpr auto EXPECTED_DRIFT = Fabs(EARTH::ROTATIONAL_RATE * EARTH::IERS_DAY_SECONDS - (2.0 * PI));
    constexpr double DRIFT_TOLERANCE = 1.00000005;

    // Check alignment every period for simulation durations of at least one year
    {
        static_assert(IsVector3Near(Earth::ECI2ECEF(0.0).Rotate(Vector3::UNIT_X()), 
                                    Vector3::UNIT_X(), 1.0E-15));

        for (int Index = 1; Index < 365; ++Index)
        {
            ASSERT_TRUE(IsVector3Near(Earth::ECI2ECEF(Index * EARTH::IERS_DAY_SECONDS).Rotate(Vector3::UNIT_X()),
                                      Vector3::UNIT_X(), EXPECTED_DRIFT * Index * DRIFT_TOLERANCE));
        }
    }

    // Check alignment over a standard rotation
    {
        static_assert(IsVector3Near(Earth::ECI2ECEF(EARTH::IERS_DAY_SECONDS * 0.25).Rotate(Vector3::UNIT_X()),
                                    -Vector3::UNIT_Y(), EXPECTED_DRIFT * 0.25 * DRIFT_TOLERANCE));

        static_assert(IsVector3Near(Earth::ECI2ECEF(EARTH::IERS_DAY_SECONDS * 0.5).Rotate(Vector3::UNIT_X()),
                                    -Vector3::UNIT_X(), EXPECTED_DRIFT * 0.5 * DRIFT_TOLERANCE));

        static_assert(IsVector3Near(Earth::ECI2ECEF(EARTH::IERS_DAY_SECONDS * 0.75).Rotate(Vector3::UNIT_X()),
                                    Vector3::UNIT_Y(), EXPECTED_DRIFT * 0.75 * DRIFT_TOLERANCE)); 
    }

}


TEST(Earth, ECEF2LLA)
{
    // Equator (prime meridian)
    {
        constexpr auto ECEF = Vector3({EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, 0.0, 1.0E-8));
        static_assert(LLAVal.Lgt == 0.0);
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-8));
    }

    // Equator (anti meridian)
    {
        constexpr auto ECEF = Vector3({-EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, 0.0, 1.0E-8));
        static_assert(LLAVal.Lgt == 180.0);
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-8));
    }    

    // Equator (+90 deg)
    {
        constexpr auto ECEF = Vector3({0.0, EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, 0.0, 1.0E-8));
        static_assert(LLAVal.Lgt == 90.0);
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-8));
    }

    // Equator (-90 deg)
    {
        constexpr auto ECEF = Vector3({0.0, -EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, 0.0, 1.0E-8));
        static_assert(LLAVal.Lgt == -90.0);
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-8));
    }        

    // North Pole
    {
        constexpr auto ECEF = Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MINOR_AXIS});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, 90.0, 1.0E-8));
        static_assert(LLAVal.Lgt == 0.0);
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-8));
    }    

    // South Pole
    {
        constexpr auto ECEF = Vector3({0.0, 0.0, -EARTH::WGS84::SEMI_MINOR_AXIS});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, -90.0, 1.0E-8));
        static_assert(LLAVal.Lgt == 0.0);
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-8));
    }       

    // Brisbane
    {
        constexpr auto ECEF = Vector3({-5047162.36, 2568329.79, -2924521.17});
        constexpr auto LLAVal = Earth::ECEF2LLA(ECEF);

        static_assert(IsNear(LLAVal.Lat, -27.47, 1.0E-2));
        static_assert(IsNear(LLAVal.Lgt, 153.03, 1.0E-2));
        static_assert(IsNear(LLAVal.Alt, 0.0, 1.0E-2));
    }     
}


TEST(Earth, LLA2ECEF)
{
    constexpr auto EquatorialComponents = Earth::WGS84RadiiComponents(0.0);
    constexpr auto PolarComponents = Earth::WGS84RadiiComponents(PI * 0.5);

    // Equatorial Point (prime merdian)
    {
        constexpr auto LLACoord = LLA{.Lat = 0.0, .Lgt = 0.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, EquatorialComponents);

        constexpr auto Expected = Vector3({EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-8)); 
    }  

    // Equatorial Point (anti merdian)
    {
        constexpr auto LLACoord = LLA{.Lat = 0.0, .Lgt = 180.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, EquatorialComponents);

        constexpr auto Expected = Vector3({-EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-8)); 
    }   

    // Equatorial Point (anti merdian as negative longitude)
    {
        constexpr auto LLACoord = LLA{.Lat = 0.0, .Lgt = -180.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, EquatorialComponents);

        constexpr auto Expected = Vector3({-EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-8)); 
    }     

    // Equatorial Point (+90 deg long)
    {
        constexpr auto LLACoord = LLA{.Lat = 0.0, .Lgt = 90.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, EquatorialComponents);

        constexpr auto Expected = Vector3({0.0, EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-8)); 
    }     

    // Equatorial Point (-90 deg long)
    {
        constexpr auto LLACoord = LLA{.Lat = 0.0, .Lgt = -90.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, EquatorialComponents);

        constexpr auto Expected = Vector3({0.0, -EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-8)); 
    }           

    // North Pole
    {
        constexpr auto LLACoord = LLA{.Lat = 90.0, .Lgt = 0.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, PolarComponents);

        constexpr auto Expected = Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MINOR_AXIS});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-4)); 
    }   

    // South Pole
    {
        constexpr auto LLACoord = LLA{.Lat = -90.0, .Lgt = 0.0, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, PolarComponents);

        constexpr auto Expected = Vector3({0.0, 0.0, -EARTH::WGS84::SEMI_MINOR_AXIS});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-4)); 
    }       

    // Brisbane
    {
        constexpr auto LLACoord = LLA{.Lat = -27.47, .Lgt = 153.03, .Alt = 0.0};
        constexpr auto ECEF = Earth::LLA2ECEF(LLACoord, Earth::WGS84RadiiComponents(D2R(LLACoord.Lat)));

        constexpr auto Expected = Vector3({-5047162.36, 2568329.79, -2924521.17});

        static_assert(IsVector3Near(ECEF, Expected, 1.0E-2));
    }
}