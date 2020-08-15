#pragma once

#include <limits>
#include "math/constants.hpp"
#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "math/quaternion.hpp"
#include "ephemeris/ephemeris.hpp"

enum struct TwoBodyOrbitClassification
{
    INVALID,    
    CIRCULAR_EQUATORIAL,
    CIRCULAR_INCLINED,
    ELLIPTICAL_EQUATORIAL,
    ELLIPTICAL_INCLINED,
    PARABOLIC,
    HYPERBOLIC
};

/** 
 * Keplerian orbital elements
 */
struct KeplerianElements
{
    // Semiparameter, describes the size of the conic section (m)
    double SemiParameter = 0.0;

    // Major radius of the orbit (m)
    double SemiMajorAxis = 0.0;

    // Orbital eccentricity >=0
    double Eccentricity = 0.0;

    // Tilt of the orbital plane, measured from
    // the unit vector k, to the angular momentum vector
    // (0 - 180 deg)
    double Inclination = 0.0;

    // Right Ascenscion of the ascending node
    // Angle in the equatorial plane measured positive
    // eastward from the i unit vector to the location
    // of the ascending node (0 - 360 deg)
    double Node = 0.0;

    // Argument of perigee, angle measured from the ascending
    // node in the direction of satellite motion (0 - 360 deg)
    double ArgumentPerigee = 0.0;

    // True anomoly, determines satellite position relative to
    // the location of periapsis (0 - 360 deg)
    double TrueAnomoly = 0.0;  

    // True longitude of periapsis is the angle measured eastward 
    // from the vertical equinox (0 - 360 deg)
    double TrueLongitudeOfPeriapsis = 0.0;

    // The argument of latitude is the angle measured between the ascending node
    // and the satellites position in the direction of satellite motion
    double ArgumentLatitude = 0.0;    

    // The true longitude is the angle measured eastward from the I axis to the position
    // of the satellite
    double TrueLongitude = 0.0;

    /**
     *  Returns true if the orbit is valid    
     */
    constexpr bool IsValid() const noexcept
    {
        return (SemiMajorAxis > 0.0);
    }

    /** 
     * Returns true if the orbit is closed
     */
    constexpr bool IsClosed() const noexcept
    {
        return (Eccentricity < 1.0);
    }

    /** 
     * Returns true if the orbit is circular in any plane
     */
    constexpr bool IsCircular() const noexcept
    {
        return (Eccentricity == 0.0);
    }    

    /** 
     * Returns true if the orbit is parabolic in any plane
     */
    constexpr bool IsParabolic() const noexcept
    {
        return (Eccentricity == 1.0);
    }    
    
    /** 
     * Returns true if the orbit is hyperbolic in any plane
     */
    constexpr bool IsHyperbolic() const noexcept
    {
        return (Eccentricity > 1.0);
    }    

    /** 
     * Returns true if the orbit lies exactly in the equatorial plane
     */
    constexpr bool IsEquatorial() const noexcept
    {
        return ((Inclination == 0.0) || (Inclination == 180.0));
    }

    /** 
     * Gets the classification of the orbit
     */
    constexpr TwoBodyOrbitClassification Classification() const noexcept
    {
        if (IsValid() == false)
        {
            return TwoBodyOrbitClassification::INVALID;    
        }

        if (Eccentricity > 1.0)
        {
            return TwoBodyOrbitClassification::HYPERBOLIC;    
        }
        else if (Eccentricity == 1.0)
        {
            return TwoBodyOrbitClassification::PARABOLIC;    
        }
        else if (Eccentricity > 0.0)
        {
            if (Inclination == 0.0)    
            {
                return TwoBodyOrbitClassification::ELLIPTICAL_EQUATORIAL;
            }
            else
            {
                return TwoBodyOrbitClassification::ELLIPTICAL_INCLINED;
            }
        }
        else
        {
            if (Inclination == 0.0)    
            {
                return TwoBodyOrbitClassification::CIRCULAR_EQUATORIAL;    
            }
            else
            {
                return TwoBodyOrbitClassification::CIRCULAR_INCLINED;    
            }
        }
    }    
};

/** 
 * Compute the keplerian orbital elements given a position and velocity state vector.
 * Assumes a two body problem
 * Based upon Algorithm 9 as detailed in "Fundamentals of Astrodynamics and applications"
 * David A. Vallado, 4th Edition
 * Inputs:
 * @param Position, instantaneous position (x, y, z) (m) of the orbiting satellite in the body centred frame
 * @param Velocity, instantaneous velocity (x, y, z) (m/s) of the orbiting satellite in the body centred frame
 * @param GravitationalParameter, newtonian gravitational parameter of the central body (m3 / s2)
 * 
 * @return, KeplerianElements
 */
constexpr KeplerianElements Newtonian2Kepler(const Vector3& Position, const Vector3& Velocity, double GravitationalParameter) noexcept
{
    auto Radius = Position.Norm();

    // Invalid orbit
    if ((Radius == 0.0) || (Velocity.NormSquared() == 0.0))
    {
        return KeplerianElements{};
    }

    KeplerianElements Result;    

    auto SpeedSquared = Velocity.NormSquared();
    auto AngularMomentum     = Vector3::Cross(Position, Velocity);
    auto AngularMomentumMagn = AngularMomentum.Norm();
    auto NodeVector          = Vector3::Cross(Vector3::UNIT_Z(), AngularMomentum);
    auto NodeVectorMagn      = NodeVector.Norm();
    auto KinematicDot = Vector3::Dot(Position, Velocity);
    auto EccentricityVector  = ((SpeedSquared - GravitationalParameter / Radius) * Position 
        - KinematicDot * Velocity) / GravitationalParameter;
    auto MechanicalEnergy = 0.5 * SpeedSquared - GravitationalParameter / Radius;

    // Calculate eccentricity    
    Result.Eccentricity = EccentricityVector.Norm();

    // Calculate orbit magnitude
    if (Result.Eccentricity == 1.0) // Orbit is a parabola
    {
        Result.SemiParameter = AngularMomentumMagn * AngularMomentumMagn / GravitationalParameter;
        Result.SemiMajorAxis = std::numeric_limits<double>::infinity();    
    }
    else // Otherwise
    {
        Result.SemiMajorAxis = -GravitationalParameter / (2.0 * MechanicalEnergy);
        Result.SemiParameter = Result.SemiMajorAxis * (1.0 - Result.Eccentricity * Result.Eccentricity);
    }

    // Orbital inclination from equatorial plane
    Result.Inclination      = R2D(Acos(AngularMomentum.Z / AngularMomentumMagn));

    // Right ascension of ascending node
    if (NodeVectorMagn > 0.0)
    {
        Result.Node             = R2D(Acos(NodeVector.X / NodeVectorMagn));
        if (NodeVector.Y < 0.0)
        {
            Result.Node = 360.0 - Result.Node;
        }

        // Argument of the perigee
        Result.ArgumentPerigee  = R2D(Acos(Vector3::Dot(NodeVector, EccentricityVector) / (Result.Eccentricity * NodeVectorMagn)));
        if (EccentricityVector.Z < 0.0)
        {
            Result.ArgumentPerigee = 360.0 - Result.ArgumentPerigee;    
        }       

        // Special case parameter - circular inclined
        Result.ArgumentLatitude = R2D(Acos(Vector3::Dot(NodeVector, Position) / (NodeVectorMagn * Radius)));
        if (Position.Z < 0.0)
        {
            Result.ArgumentLatitude = 360.0 - Result.ArgumentLatitude;
        }         
    }

    if (Result.Eccentricity > 0.0)
    {
        // Position within the orbit
        Result.TrueAnomoly = R2D(Acos(Vector3::Dot(EccentricityVector, Position) / (Result.Eccentricity * Radius)));
        if (KinematicDot < 0.0)
        {
            Result.TrueAnomoly = 360.0 - Result.TrueAnomoly;    
        }
            
        // Special case parameter - elliptical equatorial            
        Result.TrueLongitudeOfPeriapsis = R2D(Acos(EccentricityVector.X / Result.Eccentricity));
        if (EccentricityVector.Y < 0.0)
        {
            Result.TrueLongitudeOfPeriapsis = 360.0 - Result.TrueLongitudeOfPeriapsis;    
        }
    }        

    // Special case parameter - circular equatorial
    Result.TrueLongitude = R2D(Acos(Position.X / Radius));
    if (Position.Y < 0.0)
    {
        Result.TrueLongitude = 360.0 - Result.TrueLongitude;    
    }

    return Result;    
}

/** 
 * Coverts keplerian orbital elements to position and velocity state vectors
 * Assumes a 2 body problem. Assumes an aberration free problem to compute light time
 * Based upon Algorithm 10 as detailed in "Fundamentals of Astrodynamics and applications"
 * David A. Vallado, 4th Edition
 * Inputs:
 * @param Orbit, keplerian orbital elements
 * @param GravitationalParameter, newtonian gravitational parameter of the central body (m3 / s2)
 * 
 * @return, EphemerisState (Position, Velocity, LightTime) 
 */
constexpr EphemerisState Kepler2Newtonian(const KeplerianElements& Orbit, double GravitationalParameter) noexcept
{
    const auto Classification = Orbit.Classification();

    double UseAnomoly = 0.0, UseNode = 0.0, UsePerigee = 0.0;
    if (Classification == TwoBodyOrbitClassification::CIRCULAR_EQUATORIAL)
    {
        UseAnomoly = Orbit.TrueLongitude;
    }
    else if (Classification == TwoBodyOrbitClassification::CIRCULAR_INCLINED)
    {
        UseNode = Orbit.Node;
        UseAnomoly = Orbit.ArgumentLatitude;
    }
    else if (Classification == TwoBodyOrbitClassification::ELLIPTICAL_EQUATORIAL)
    {
        UsePerigee = Orbit.TrueLongitudeOfPeriapsis;    
        UseAnomoly = Orbit.TrueAnomoly;
    }
    else
    {
        UsePerigee = Orbit.ArgumentPerigee;
        UseNode = Orbit.Node;
        UseAnomoly = Orbit.TrueAnomoly;
    }

    const auto CosAnomoly = Cos(D2R(UseAnomoly));
    const auto SinAnomoly = Sin(D2R(UseAnomoly));
    const auto Distance = Orbit.SemiParameter / (1.0 + Orbit.Eccentricity * CosAnomoly);
    const auto Coeff2 = Sqrt(GravitationalParameter / Orbit.SemiParameter);

    // Newtonian state within the orbital plane
    const auto PosPQW = Vector3({Distance * CosAnomoly, Distance * SinAnomoly, 0.0});
    const auto VelPQW = Vector3({-Coeff2 * SinAnomoly, Coeff2 * (Orbit.Eccentricity + CosAnomoly), 0.0});

    // Rotation from orbital plane to central body frame
    const auto Rot = 
        Quaternion::FromVectorAngle(Vector3::UNIT_Z(), -D2R(UsePerigee)) *
        Quaternion::FromVectorAngle(Vector3::UNIT_X(), -D2R(Orbit.Inclination)) *
        Quaternion::FromVectorAngle(Vector3::UNIT_Z(), -D2R(UseNode));

    // Newtonian state
    return EphemerisState{.Pos = Rot.Rotate(PosPQW), .Vel = Rot.Rotate(VelPQW), .LightTime =  Distance / SPEED_LIGHT};
}
