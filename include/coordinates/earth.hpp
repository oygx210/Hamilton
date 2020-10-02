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
    /**
    * Calculate the radius of the earth at the given
    * `GeodeticInclination` (rad) assuming the WGS84
    * ellipsoid
    * 
    * Accurate to around 0.1 - 1 mm
    * 
    * @param GeodeticInclination Geodetic Inclination
    * @return Instantaneous Radius at the given Inclination
    */
    inline constexpr double WGS84Radius(double GeodeticInclination) noexcept
    {
        using namespace Earth::WGS84;    

        const auto S = Sin(GeodeticInclination);
        const auto C = Cos(GeodeticInclination);
        const auto S2 = S * S;
        const auto C2 = C * C;
        constexpr auto A2 = SEMI_MAJOR_AXIS * SEMI_MAJOR_AXIS;
        constexpr auto B2 = SEMI_MINOR_AXIS * SEMI_MINOR_AXIS;

        return Sqrt((A2 * A2 * C2 + B2 * B2 * S2) / (A2 * C2 + B2 * S2));
    }

    /**
    * Calculate the azimuthal and inclined radial components
    * needed to convert between ECEF and LLA
    * 
    * @param Inclination Geodetic Latitude (rad)
    * @return EllispoidRadii components
    */
    inline constexpr EllipsoidRadii WGS84RadiiComponents(double Inclination) noexcept
    {
        using namespace Earth::WGS84;    

        const auto SInc = Sin(Inclination);
        const auto N = SEMI_MAJOR_AXIS / (Sqrt(1.0 - ECCSQ * SInc * SInc));

        return EllipsoidRadii{.Azimuthal = N, .Inclined = N * (1.0 - ECCSQ)};
    }

    /**
    * Calculates Latitude (deg), longitude (deg), altitude (m)
    * on the WGS84 Earth model from a given ECEF co-ordinate. 
    * 
    * Uses Bowring's method to iterate towards the correct latitude.
    * 
    * Resulting LLA should be accurate to 8 decimal point of latitude, or
    * about 1.1 mm
    * 
    * @param ECEF Earth Centred Earth Fixed co-ordinates of some point P
    * @return Point P in Latitude (deg), Longitude (deg), Altitude (m) co-ordinates
    */
    inline constexpr LLA ECEF2LLA(const Vector3& ECEF) noexcept
    {
        using namespace Earth::WGS84;    

        // Target about 1mm precision
        constexpr double TOLERANCE = 1.0E-8;

        // Break loop in case of non convergence    
        constexpr int MAXITER = 32;

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
        int It = 0;
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

    /**
    * Calculate earth centred, eartg fixed co-ordinates from latitude, 
    * longitude, altitude and ellipsoid radial components
    * 
    * @param LLAVal Point P in Latitude (deg), Longitude (deg), Altitude (m)
    * @param Radii: Prime vertical radius of curvature (and its complement) 
    * at the given lla
    * @return Point P in Earth Centred Earth Fixed co-ordinates
    */
    inline constexpr Vector3 LLA2ECEF(const LLA& LLAVal, const EllipsoidRadii& Radii) noexcept
    {
        return LLA2BCBF(LLAVal, Radii);
    }

    /**
    * Calculate a quaternion relating ECI to ECEF
    * @param TimeOffset Seconds offset in the current epoch
    * @return Quaternion Earth Centred Inertial to Earth Centred Earth Fixed
    */
    inline constexpr Quaternion ECI2ECEF(double TimeOffset) noexcept
    {
        return BCI2BCBF(Earth::ROTATIONAL_RATE, TimeOffset);
    }    

    /**
    * Calculates a quaternion relating earth centred, earth fixed co-ordinates
    * to East, North, Up co-ordinates
    * @param LLAVal Point P in Latitude (deg), Longitude (deg), Altitude (m) co-ordinates
    * @return Quaternion Earth Centred Earth Fixed to East North Up Co-ordinates
    */
    inline constexpr Quaternion QuatECEF2ENU(const LLA& LLAVal) noexcept
    {
        return QuatBCBF2ENU(LLAVal);
    }

    /**
    * Calculates a quaternion relating earth centred, earth fixed co-ordinates
    * to East, North, Up co-ordinates (radians)
    * @param LLAVal Point P in Latitude (rad), Longitude (rad), Altitude (m) co-ordinates
    * @return Quaternion Earth Centred Earth Fixed to East North Up Co-ordinates
    */
    inline constexpr Quaternion QuatECEF2ENU(const LLARad& LLAVal) noexcept
    {
        return QuatBCBF2ENU(LLAVal);
    }    
}