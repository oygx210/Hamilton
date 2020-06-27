#include "gtest/gtest.h"

#include "tests/test_utils.hpp"
#include "math/constants.hpp"
#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "coordinates/general.hpp"
#include "coordinates/earth.hpp"

TEST(Coordinates, RotateTranslate)
{
    // Rotate (1, 0, 0) pi/4 radians in the x-y plane and translate by (1, 1, 1)
    {
        constexpr auto V1 = Vector3::UNIT_X();
        constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), PI * 0.25);
        constexpr auto V2 = RotateTranslate(V1, Q, Vector3({1.0, 1.0, 1.0}));
        
        static_assert(IsVector3Near(V2, Vector3({1.0 + 1.0 / Sqrt(2.0), 1.0 - 1.0 / Sqrt(2.0), 1.0}), 1.0E-15));
    }

    // Rotate (0, 1, 0) by -pi/4 using an inverse rotation and translate by (1, 1, 1)
    {
        constexpr auto V1 = Vector3::UNIT_Y();
        constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), PI * 0.25);
        constexpr auto V2 = RotateInvTranslate(V1, Q, Vector3({1.0, 1.0, 1.0}));
        
        static_assert(IsVector3Near(V2, Vector3({1.0 - 1.0 / Sqrt(2.0), 1.0 + 1.0 / Sqrt(2.0), 1.0}), 1.0E-15));
    }
}

TEST(Coordinates, QuatBCBF2ENU)
{
    // along prime meridian
    {
        constexpr auto Q = QuatBCBF2ENU({.LatRad = 0.0, .LgtRad = 0.0});

        constexpr auto ENU1 = Q.Rotate(Vector3::UNIT_X());
        static_assert(IsVector3Near(ENU1, Vector3::UNIT_Z(), 1.0E-15)); // x axis should be directly Up

        constexpr auto ENU2 = Q.Rotate(-Vector3::UNIT_X());
        static_assert(IsVector3Near(ENU2, -Vector3::UNIT_Z(), 1.0E-15)); // -x axis should be directly down

        constexpr auto ENU3 = Q.Rotate(Vector3::UNIT_Y());
        static_assert(IsVector3Near(ENU3, Vector3::UNIT_X(), 1.0E-15)); // y axis should point directly East

        constexpr auto ENU4 = Q.Rotate(-Vector3::UNIT_Y());
        static_assert(IsVector3Near(ENU4, -Vector3::UNIT_X(), 1.0E-15)); // -y axis should point directly west

        constexpr auto ENU5 = Q.Rotate(Vector3::UNIT_Z());
        static_assert(IsVector3Near(ENU5, Vector3::UNIT_Y(), 1.0E-15)); // z axis should point directly North

        constexpr auto ENU6 = Q.Rotate(-Vector3::UNIT_Z());
        static_assert(IsVector3Near(ENU6, -Vector3::UNIT_Y(), 1.0E-15)); // z axis should point directly south
    }
}

// BCBF to ENU
TEST(Coordinates, BCBF2ENU)
{
    // prime meridian along equator
    {
        constexpr auto ECEF = Vector3({EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});
        constexpr auto QECEF2ENU = QuatBCBF2ENU({.LatRad = 0.0, .LgtRad = 0.0});

        // Take origin at BCBF origin
        constexpr auto ENU1 = BCBF2ENU(QECEF2ENU, ECEF, Vector3::ZERO());
        static_assert(IsVector3Near(ENU1, Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MAJOR_AXIS}), 1.0E-8));

        // Take origin at earth radius        
        constexpr auto ENU2 = BCBF2ENU(QECEF2ENU, ECEF, ECEF);
        static_assert(IsVector3Near(ENU2, Vector3::ZERO(), 1.0E-8));
    }

    // anti meridian along equator
    {
        constexpr auto ECEF = Vector3({-EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0, 0.0});
        constexpr auto QECEF2ENU = QuatBCBF2ENU({.LatRad = 0.0, .LgtRad = PI});

        // Take origin at BCBF origin
        constexpr auto ENU1 = BCBF2ENU(QECEF2ENU, ECEF, Vector3::ZERO());
        static_assert(IsVector3Near(ENU1, Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MAJOR_AXIS}), 1.0E-8));

        // Take origin at earth radius        
        constexpr auto ENU2 = BCBF2ENU(QECEF2ENU, ECEF, ECEF);
        static_assert(IsVector3Near(ENU2, Vector3::ZERO(), 1.0E-8));
    }    

    // +90 deg from prime meridian, along equator
    {
        constexpr auto ECEF = Vector3({0.0, EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0});
        constexpr auto QECEF2ENU = QuatBCBF2ENU({.LatRad = 0.0, .LgtRad = 0.5 * PI});

        // Take origin at BCBF origin
        constexpr auto ENU1 = BCBF2ENU(QECEF2ENU, ECEF, Vector3::ZERO());
        static_assert(IsVector3Near(ENU1, Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MAJOR_AXIS}), 1.0E-8));

        // Take origin at earth radius        
        constexpr auto ENU2 = BCBF2ENU(QECEF2ENU, ECEF, ECEF);
        static_assert(IsVector3Near(ENU2, Vector3::ZERO(), 1.0E-9));     
    }    

    // -90 deg from prime meridian, along equator
    {
        constexpr auto ECEF = Vector3({0.0, -EARTH::WGS84::SEMI_MAJOR_AXIS, 0.0});
        constexpr auto QECEF2ENU =  QuatBCBF2ENU({.LatRad = 0.0, .LgtRad = -0.5 * PI});

        // Take origin at BCBF origin
        constexpr auto ENU1 = BCBF2ENU(QECEF2ENU, ECEF, Vector3::ZERO());
        static_assert(IsVector3Near(ENU1, Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MAJOR_AXIS}), 1.0E-8));

        // Take origin at earth radius        
        constexpr auto ENU2 = BCBF2ENU(QECEF2ENU, ECEF, ECEF);
        static_assert(IsVector3Near(ENU2, Vector3::ZERO(), 1.0E-9));
    }  

    // Brisbane
    {
        constexpr auto LLABrisbane = LLA{.Lat = -27.47, .Lgt = 153.03, .Alt = 0.0};
        constexpr auto ECEFBrisbane = Earth::LLA2ECEF(LLABrisbane, Earth::WGS84RadiiComponents(D2R(LLABrisbane.Lat)));
        constexpr auto QECEF2ENU = QuatBCBF2ENU(LLABrisbane);

        constexpr auto SinAzm = Sin(D2R(LLABrisbane.Lgt));
        constexpr auto CosAzm = Cos(D2R(LLABrisbane.Lgt));
        constexpr auto SinInc = Sin(D2R(LLABrisbane.Lat));
        constexpr auto CosInc = Cos(D2R(LLABrisbane.Lat));

        // Vector due East
        constexpr auto East = Vector3({-SinAzm, CosAzm, 0.0});

        // Vector due North
        constexpr auto North = Vector3({-CosAzm * SinInc, -SinAzm * SinInc, CosInc});

        // Vector directly Up
        constexpr auto Up = Vector3({CosAzm * CosInc, SinAzm * CosInc, SinInc});

        // Take an observed point relative to Brisbane 
        // + 100m Up + 200m East - 300m North
        constexpr auto Target = ECEFBrisbane 
            + Up * 100.0
            + East * 200.0
            + North * -300.0;

        constexpr auto ENUTarget = BCBF2ENU(QECEF2ENU, Target, ECEFBrisbane);
        static_assert(IsVector3Near(ENUTarget, Vector3({200.0, -300.0, 100.0}), 1.0E-8 ));
    }    
}
//
// ENU to BCBF
TEST(Coordinates, ENU2BCBF)
{
    // Brisbane    
    {
        constexpr auto LLABrisbane = LLA{.Lat = -27.47, .Lgt = 153.03, .Alt = 0.0};
        constexpr auto ECEFBrisbane = Earth::LLA2ECEF(LLABrisbane, Earth::WGS84RadiiComponents(D2R(LLABrisbane.Lat)));
        constexpr auto Q = QuatBCBF2ENU(LLABrisbane);

        constexpr auto ENUTarget = Vector3({200.0, -300.0, 100.0});
        constexpr auto ECEFTarget = ENU2BCBF(Q, ENUTarget, ECEFBrisbane);
        constexpr auto ECEFExpected = Vector3({-5047208.808846405, 2568129.015902008, -2924833.469542103});
        static_assert(IsVector3Near(ECEFExpected, ECEFTarget, 1.0E-8));
    }
}

// Calculate azimuth, elevation, range from a set of ENU
// Co-ordinates
TEST(Coordinates, CalculateLTPRange)
{
    // Looking at the same point    
    {
        constexpr auto P1 = Vector3({1000.0, 2000.0, EARTH::WGS84::SEMI_MAJOR_AXIS});
        constexpr auto P2 = P1;
        constexpr auto AER = CalculateLTPRange(P1, P2);
        static_assert(AER.Rad == 0.0);
        static_assert(AER.Inc == 0.0);
        static_assert(AER.Azm == 0.0);
    }   

    // Looking at a south Easterly point from the equator    
    {
        constexpr auto P1 = Vector3({0.0, 0.0, EARTH::WGS84::SEMI_MAJOR_AXIS});
        constexpr auto P2 = Vector3({2000.0, -2000.0, EARTH::WGS84::SEMI_MAJOR_AXIS + 8000.0});

        constexpr auto AER = CalculateLTPRange(P1, P2);
        static_assert(AER.Rad == (P1 - P2).Norm());
        static_assert(AER.Azm == -PI * 0.25); // South East
        static_assert(AER.Inc == Atan2(4.0, Sqrt(2.0)) );
    }

    // Looking at a generic nearby point
    {
        constexpr auto ENUTarget = Vector3({200.0, -300.0, 100.0});
        
        constexpr auto AER = CalculateLTPRange(Vector3::ZERO(), ENUTarget);

        static_assert(IsNear(AER.Rad, ENUTarget.Norm(), 1.0E-9));
        static_assert(IsNear(AER.Azm, Atan2(-300.0, 200.0), 1.0E-15));
        static_assert(IsNear(AER.Inc, Atan2(100.0, Sqrt(Pow(200.0, 2) + Pow(-300, 2))), 1.0E-15));
    }   

    // Calculation from BCBF co-ordinates
    {
        constexpr auto ECEFBrisbane = Vector3({-5047162.363234413, 2568329.786714818, -2924521.165386110});
        constexpr auto ECEFTarget   = Vector3({-5047208.808846405, 2568129.015902008, -2924833.469542103});
        constexpr auto Q            = Earth::QuatECEF2ENU(Earth::ECEF2LLA(ECEFBrisbane));
        constexpr auto AER = CalculateLTPRange(ECEFBrisbane, ECEFTarget, Q);

        static_assert(IsNear(AER.Rad, (ECEFBrisbane - ECEFTarget).Norm(), 1.0E-11));
        static_assert(IsNear(AER.Azm, Atan2(-300.0, 200.0), 1.0E-11));
        static_assert(IsNear(AER.Inc, Atan2(100.0, Sqrt(Pow(200.0, 2) + Pow(-300, 2))), 1.0E-11));
    }    
}
