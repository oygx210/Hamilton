#pragma once

/* 
 * A variety of generalised co-ordinate transformation helpers
 */

#include "math/quaternion.hpp"
#include "math/axis3.hpp"
#include "math/vector3.hpp"
#include "math/spherical.hpp"
#include "math/lla.hpp"
#include "math/ellipsoid.hpp"

/* 
 * Rotate a vector u in frame A to frame B using the quaternion qA2B,
 * then offset it by some vector v in frame B
 * 
 * Inputs:
 *  vectA   : original vector u_A in frame A
 *  quatA2B : quaternion describing rotation between frames A -> B
 *  transB  : offset v_B to translate u_B by
 */
inline constexpr Vector3 RotateTranslate(
    const Vector3& VectA,
    const Quaternion& QuatA2B, 
    const Vector3& TransB) noexcept
{
    return QuatA2B.Rotate(VectA) + TransB;
}

/* 
 * Rotate a vector u in frame A to frame B using the quaternion qB2A,
 * then offset it by some vector v in frame B
 * 
 * Inputs:
 *  vectA   : original vector u_A in frame A
 *  quatB2A : quaternion describing rotation between frames B -> A
 *  transB  : offset v_B to translate u_B by
 */
inline constexpr Vector3 RotateInvTranslate(
    const Vector3& VectA,
    const Quaternion& QuatB2A,
    const Vector3& TransB) noexcept
{
    return QuatB2A.RotateInv(VectA) + TransB;
}

/*
 * Translates vector u in frame A by an offset v in the same frame
 * then rotates it to frame B via the quaternion quatA2B
 * 
 * Inputs:
 *  vectA   : original vector u_A in frame A
 *  transA  : offset v_A to translate u_A by 
 *  quatA2B : quaternion describing rotation between frames A -> B
 */
inline constexpr Vector3 TranslateRotate(const Vector3& VectA, 
                                         const Vector3& TransA, 
                                         const Quaternion& QuatA2B) noexcept
{
    return QuatA2B.Rotate(VectA + TransA);
}

/*
 * Translates vector u in frame A by an offset v in the same frame
 * then rotates it to frame B via the quaternion quatB2A
 * 
 * Inputs:
 *  vectA   : original vector u_A in frame A
 *  transA  : offset v_A to translate u_A by 
 *  quatB2A : quaternion describing rotation between frames B -> A 
 */
inline constexpr Vector3 TranslateRotateInv(const Vector3& VectA, 
                                            const Vector3& TransA, 
                                            const Quaternion& QuatB2A) noexcept
{
    return QuatB2A.RotateInv(VectA + TransA);
}

/* 
 * Calculates a quaternion relating a Body Centred Inertial 
 * to Body centred, body fixed frame
 * Inputs:
 * rotationalRate: rotational velocity (rad/s) about the z axis
 * offsetTime    : time since alignment of BCBF with BCI (s)
 */
inline constexpr Quaternion BCI2BCBF(double RotationalRate, double OffsetTime) noexcept
{
    return Quaternion::FromVectorAngle(
        Vector3::UNIT_Z(),
        RotationalRate * OffsetTime);
}

/* 
 * Calculate spherical coordinates from cartesian
 * Inputs:
 * cartesian: (x,y,z) co-ordinates
 */
inline constexpr Spherical Cart2Sph(const Vector3& Cartesian) noexcept
{
    const auto Radius = Cartesian.Norm();

    return Spherical {.Rad = Radius, 
                      .Azm = Atan2(Cartesian.Y, Cartesian.X), 
                      .Inc = Atan2(Cartesian.Z, Radius)};
}

/* 
 * Calculate Cartesion co-ordinates from spherical
 * Inputs:
 * spherical: (r, azm (rad), inc (rad)) co-ordinates
 */
inline constexpr Vector3 Sph2Cart(const Spherical& Spherical) noexcept
{
    const auto STheta = Sin(Spherical.Azm);
    const auto CTheta = Cos(Spherical.Azm);
    const auto CPhi   = Cos(Spherical.Inc);

    return Vector3({.X = Spherical.Rad * CTheta * CPhi,
                    .Y = Spherical.Rad * STheta * CPhi,
                    .Z = Spherical.Rad * Sin(Spherical.Inc)});
}

/* 
 * Calculate earth centred, eartg fixed co-ordinates from latitude, 
 * longitude, altitude and ellipsoid radial components
 * Inputs:
 *     LLAVal: Latitude, Longitude, Altitude
 *     Radii: prime vertical radius of curvature (and its complement) 
 * at the given lla
 */
inline constexpr Vector3 LLA2BCBF(const LLA& LLAVal, const EllipsoidRadii& Radii) noexcept
{
    const auto CTheta = Cos(D2R(LLAVal.Lgt));
    const auto STheta = Sin(D2R(LLAVal.Lgt));
    const auto CPhi = Cos(D2R(LLAVal.Lat));
    const auto SPhi = Sin(D2R(LLAVal.Lat));

    return Vector3({(Radii.Azimuthal + LLAVal.Alt) * CTheta * CPhi,
                    (Radii.Azimuthal + LLAVal.Alt) * STheta * CPhi,
                    (Radii.Inclined + LLAVal.Alt) * SPhi});
}

/* 
 * Calculates a quaternion relating body centred, body fixed co-ordinates
 * to East, North, Up co-ordinates
 */
inline constexpr Quaternion QuatBCBF2ENU(const LLA& LLAVal) noexcept
{
    return Quaternion::FromVectorAngle(Vector3::UNIT_Z(), 0.5 * PI + D2R(LLAVal.Lgt)) *
           Quaternion::FromVectorAngle(Vector3::UNIT_X(), 0.5 * PI - D2R(LLAVal.Lat));
           
}

/* 
 * Calculates a quaternion relating body centred, body fixed co-ordinates
 * to East, North, Up co-ordinates
 */
inline constexpr Quaternion QuatBCBF2ENU(const LLARad& LLAVal) noexcept
{
    return Quaternion::FromVectorAngle(Vector3::UNIT_Z(), 0.5 * PI + LLAVal.LgtRad) *
           Quaternion::FromVectorAngle(Vector3::UNIT_X(), 0.5 * PI - LLAVal.LatRad);
           
}

/* 
 * Computes East, North, Up components from BCBF and a rotation
 * quaternion
 * 
 * Inputs:
 * quatBCBF2ENU: quaternion describing rotation BCBF -> ENU
 * BCBF        : co-ordinate to be transformed in the BCBF frame
 * BCBFOrigin  : Origin of the local ENU frame in the BCBF frame
 */
inline constexpr Vector3 BCBF2ENU(
    const Quaternion& QuatBCBF2ENU, 
    const Vector3& BCBF, 
    const Vector3& BCBFOrigin) noexcept
{
    return TranslateRotate(BCBF, -BCBFOrigin, QuatBCBF2ENU);
}

/* 
 * Computes body centred, body fixed components from East North Up co-ordinates and a 
 * rotationan quaternion
 * 
 * Inputs:
 *     QuatBCBF2ENU: quaternion describing rotation BCBF -> ENU
 *     ENU         : co-ordinate to be transformed in the local ENU frame
 *     BCBFOrigin  : Origin of the local ENU frame in the BCBF frame
 */
inline constexpr Vector3 ENU2BCBF(const Quaternion& QuatBCBF2ENU, 
                                  const Vector3& ENU, 
                                  const Vector3& BCBFOrigin) noexcept
{
    return RotateInvTranslate(ENU, QuatBCBF2ENU, BCBFOrigin);
}

/* 
 * Calculate the range (m), 
 * azimuth (rad) from due North and
 * inclination (rad) from North-East plane
 * between a source point and a target point 
 * described in ENU frame.
 */
inline constexpr Spherical CalculateLTPRange(
    const Vector3& SourceENU, 
    const Vector3& TargetENU) noexcept
{
    const auto DeltaENU = TargetENU - SourceENU;

    const auto TangentialRadiusSq = Pow(DeltaENU.X, 2) 
                                  + Pow(DeltaENU.Y, 2);
    const auto TangentialRadius = Sqrt(TangentialRadiusSq);
    const auto Range = Sqrt(TangentialRadiusSq + Pow(DeltaENU.Z, 2));

    const auto Azimuth = Atan2(DeltaENU.Y, DeltaENU.X);
    const auto Elevation = Atan2(DeltaENU.Z, TangentialRadius);

    return Spherical{.Rad = Range, .Azm = Azimuth, .Inc = Elevation};
}

/* 
 * Calculate the range (m), azimuth (rad) and inclination (rad)
 * from the North East plane from a source and target point.
 * Converts to local tangent plane points as an 
 * intermediate step
 * 
 * Inputs: 
 * SourceBCBF: Source (observer) point in body centred, body fixed frame
 * TargetBCBF: Target (observed) point in body centred, body fixed frame
 * QuatBCBF2ENU: Quaternion describing rotation between source BCBF point
 *     and the local tangent plane ENU co-ordinates at this point
 */
inline constexpr Spherical CalculateLTPRange(
    const Vector3& SourceBCBF, 
    const Vector3& TargetBCBF, 
    const Quaternion& QuatBCBF2ENU) noexcept
{
    const auto DeltaENU = BCBF2ENU(QuatBCBF2ENU, TargetBCBF, SourceBCBF);

    const auto TangentialRadiusSq = Pow(DeltaENU.X, 2) + Pow(DeltaENU.Y, 2);
    const auto TangentialRadius = Sqrt(TangentialRadiusSq);
    const auto Range = Sqrt(TangentialRadiusSq + Pow(DeltaENU.Z, 2));

    const auto Azimuth = Atan2(DeltaENU.Y, DeltaENU.X);
    const auto Elevation = Atan2(DeltaENU.Z, TangentialRadius);

    return Spherical{.Rad = Range, .Azm = Azimuth, .Inc = Elevation};    
}                                   