#pragma once

#include "math/constants.hpp"
#include "math/core_math.hpp"
#include "math/ellipsoid.hpp"
#include "math/lla.hpp"
#include "math/vector3.hpp"
#include "math/quaternion.hpp"
#include "coordinates/general.hpp"

namespace Earth
{
    /*
    * Calculate the radius of the earth at the given
    * geodetic inclination (rad) assuming the WGS84
    * ellipsoid
    * 
    * Accurate to around 0.1 - 1 mm
    */
    inline constexpr double WGS84Radius(double GeodeticInclination)
    {
        using namespace EARTH::WGS84;    

        auto S = Sin(GeodeticInclination);
        auto C = Cos(GeodeticInclination);
        auto S2 = S * S;
        auto C2 = C * C;
        constexpr auto A2 = SEMI_MAJOR_AXIS * SEMI_MAJOR_AXIS;
        constexpr auto B2 = SEMI_MINOR_AXIS * SEMI_MINOR_AXIS;

        return Sqrt((A2 * A2 * C2 + B2 * B2 * S2) / (A2 * C2 + B2 * S2));
    }

    /*
    * Calculate the azimuthal and inclined radial components
    * needed to convert between ECEF and LLA
    * 
    * Inputs:
    * inclination: geodetic latitude 
    */
    inline constexpr EllipsoidRadii WGS84RadiiComponents(double Inclination)
    {
        using namespace EARTH::WGS84;    

        auto SInc = Sin(Inclination);
        auto N = SEMI_MAJOR_AXIS / (Sqrt(1.0 - ECCSQ * SInc * SInc));

        return EllipsoidRadii{.Azimuthal = N, .Inclined = N * (1.0 - ECCSQ)};
    }

    /*
    * Calculates Latitude (deg), longitude (deg), altitude (m)
    * on the WGS84 Earth model from a given ECEF co-ordinate
    * 
    * Uses Bowring's method to iterate towards the correct latitude
    * 
    * Resulting LLA should be accurate to 8 decimal point of latitude, or
    * about 1.1 mm
    */
    inline constexpr LLA ECEF2LLA(const Vector3& ECEF)
    {
        using namespace EARTH::WGS84;    

        // Target about 1mm precision
        constexpr double TOLERANCE = 1.0E-8;

        // Break loop in case of non convergence    
        constexpr uint8_t MAXITER = 32;

        // geodetic longitude (rad)
        const auto Longitude = Atan2(ECEF.Y, ECEF.X);

        // Equatorial plane projected length (m)
        const auto S = Sqrt(ECEF.X * ECEF.X + ECEF.Y * ECEF.Y);    

        // Use bowings method to calculate geodetic latitude
        auto Bowring = [S, ECEFZ = ECEF.Z](double Beta)
        {
            constexpr auto Coeff = ECCSQ * (1.0 - FLATTENING) / (1.0 - ECCSQ) * SEMI_MAJOR_AXIS;

            auto S3 = Sin(Beta);
            S3 *= S3 * S3;
            auto C3 = Cos(Beta);
            C3 *= C3 * C3;

            return Atan2(ECEFZ + Coeff * S3, S - ECCSQ * SEMI_MAJOR_AXIS * C3);        
        };
        
        auto Beta = Atan2(ECEF.Z, S);
        auto Latitude = Bowring(Beta);

        // Main latitude convergence loop
        auto Delta = 2.0 * TOLERANCE;    
        uint8_t It = 0;
        while (Delta > TOLERANCE)
        {
            // Failed to converge
            if (++It == MAXITER)
            {
                printf("%s(%d) ERROR: Failed to calculate ECEF2LLA for ECEF=%s\n", __FILE__, __LINE__, ECEF.ToString().c_str());
                return LLA {.Lat = 0.0, .Lgt = 0.0, .Alt = 0.0};
            }
                
            Beta = Atan2((1.0 - FLATTENING) * Sin(Latitude), Cos(Latitude));
            auto NewLat = Bowring(Beta);
            Delta = Fabs(NewLat - Latitude);
            Latitude = NewLat;
        }

        const auto SinLat = Sin(Latitude);
        
        const auto VerticalPrime = SEMI_MAJOR_AXIS / Sqrt(1.0 - ECCSQ * Pow(SinLat, 2));
        const auto Altitude = S * Cos(Latitude) + (ECEF.Z + ECCSQ * VerticalPrime * SinLat) * SinLat - VerticalPrime;

        return LLA {.Lat = R2D(Latitude), .Lgt = R2D(Longitude), .Alt = Altitude};
    }

    /* 
    * Calculate earth centred, eartg fixed co-ordinates from latitude, 
    * longitude, altitude and ellipsoid radial components
    * Inputs:
    * lla: Latitude, Longitude, Altitude
    * radii: prime vertical radius of curvature (and its complement) 
    *       at the given lla
    */
    inline constexpr Vector3 LLA2ECEF(const LLA& LLA, const EllipsoidRadii& Radii)    
    {
        return LLA2BCBF(LLA, Radii);
    }

    /*
    * Calculate a quaternion relating ECI to ECEF
    */
    inline constexpr Quaternion ECI2ECEF(double TimeOffset)
    {
        return BCI2BCBF(EARTH::ROTATIONAL_RATE, TimeOffset);
    }    

    /* 
    * Calculates a quaternion relating earth centred, earth fixed co-ordinates
    * to East, North, Up co-ordinates
    */
    inline constexpr Quaternion QuatECEF2ENU(const LLA& LLA)
    {
        return QuatBCBF2ENU(LLA);
    }

    /* 
    * Calculates a quaternion relating earth centred, earth fixed co-ordinates
    * to East, North, Up co-ordinates (radians)
    */
    inline constexpr Quaternion QuatECEF2ENU(const LLARad& LLA)
    {
        return QuatBCBF2ENU(LLA);
    }    
}