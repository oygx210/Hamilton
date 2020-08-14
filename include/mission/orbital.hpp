#pragma once

#include <limits>
#include "math/core_math.hpp"
#include "math/vector3.hpp"

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