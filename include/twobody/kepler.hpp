#pragma once

#include "math/constants.hpp"
#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "math/quaternion.hpp"
#include "ephemeris/ephemeris.hpp"

namespace TwoBody
{
    /** 
     * Classification of a two body orbit
     */
    enum struct OrbitClassification
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
     * Kepler equations coefficients
     */
    struct CCoefficents
    {
        double C2 = 0.0;
        double C3 = 0.0;
    };    

    /** 
     * Keplerian orbital elements
     */
    struct KeplerianElements
    {
        /** Semiparameter describes the size of the conic section (m) */
        double SemiParameter = 0.0;

        /** Major radius of the orbit (m) */
        double SemiMajorAxis = 0.0;

        /** Orbital eccentricity >=0 */
        double Eccentricity = 0.0;

        /** 
         * Tilt of the orbital plane, measured from
         * the unit vector k, to the angular momentum vector
         * (0 - PI)
         */
        double Inclination = 0.0;

        /**
         * Right Ascenscion of the ascending node
         * Angle in the equatorial plane measured positive
         * eastward from the i unit vector to the location
         * of the ascending node (0 - 2 PI)
         */
        double Node = 0.0;

        /**
         * Argument of perigee, angle measured from the ascending
         * node in the direction of satellite motion (0 - 2 PI)
         */
        double ArgumentPerigee = 0.0;

        /** 
         * True anomoly, determines satellite position relative to
         * the location of periapsis (0 - 2 PI)
         */
        double TrueAnomoly = 0.0;  

        /** 
         * True longitude of periapsis is the angle measured eastward 
         * from the vertical equinox (0 - 2 PI)
         */
        double TrueLongitudeOfPeriapsis = 0.0;

        /**
         * The argument of latitude is the angle measured between the ascending node
         * and the satellites position in the direction of satellite motion
         */
        double ArgumentLatitude = 0.0;    

        /**
         * The true longitude is the angle measured eastward from the I axis to the position
         * of the satellite
         */        
        double TrueLongitude = 0.0;

        /** The gravitational parameter of the central body (m3/s2) */
        double GravitationalParameter = 0.0;        
    };

       /**
         * @param Elements Keplerian elements of the orbit 
         *  @return `true` if the orbit is valid    
         */
        constexpr bool IsValid(const KeplerianElements& Elements) noexcept
        {
            return (Elements.SemiMajorAxis > 0.0);
        }

        /** 
         * @param Elements Keplerian elements of the orbit 
         * @return `true` if the orbit is closed
         */
        constexpr bool IsClosed(const KeplerianElements& Elements) noexcept
        {
            return (Elements.Eccentricity < 1.0);
        }

        /** 
         * @param Elements Keplerian elements of the orbit 
         * @return `true` if the orbit is circular in any plane
         */
        constexpr bool IsCircular(const KeplerianElements& Elements) noexcept
        {
            return (Elements.Eccentricity == 0.0);
        }    

        /** 
         * @param Elements Keplerian elements of the orbit 
         * @return `true` if the orbit is parabolic in any plane
         */
        constexpr bool IsParabolic(const KeplerianElements& Elements) noexcept
        {
            return (Elements.Eccentricity == 1.0);
        }    
        
        /** 
         * @param Elements Keplerian elements of the orbit 
         * @return `true` if the orbit is hyperbolic in any plane
         */
        constexpr bool IsHyperbolic(const KeplerianElements& Elements) noexcept
        {
            return (Elements.Eccentricity > 1.0);
        }    

        /** 
         * @param Elements Keplerian elements of the orbit
         * @return `true` if the orbit lies exactly in the equatorial plane
         */
        constexpr bool IsEquatorial(const KeplerianElements& Elements) noexcept
        {
            return ((Elements.Inclination == 0.0) || (Elements.Inclination == 180.0));
        }

        /** 
         * @param Elements Keplerian elements of the orbit
         * @return Classification of the orbit
         */
        constexpr OrbitClassification ClassifyOrbit(const KeplerianElements& Elements) noexcept
        {
            if (IsValid(Elements) == false)            
            {
                return OrbitClassification::INVALID;    
            }

            if (IsHyperbolic(Elements))
            {
                return OrbitClassification::HYPERBOLIC;    
            }
            else if (IsParabolic(Elements))
            {
                return OrbitClassification::PARABOLIC;    
            }
            else if (Elements.Eccentricity > 0.0)
            {
                if (Elements.Inclination == 0.0)    
                {
                    return OrbitClassification::ELLIPTICAL_EQUATORIAL;
                }
                else
                {
                    return OrbitClassification::ELLIPTICAL_INCLINED;
                }
            }
            else
            {
                if (Elements.Inclination == 0.0)    
                {
                    return OrbitClassification::CIRCULAR_EQUATORIAL;    
                }
                else
                {
                    return OrbitClassification::CIRCULAR_INCLINED;    
                }
            }
        }        

    /** 
     * @param Elements Orbital elements
     * @return Period of an orbit (s)
     */
    constexpr double CalculatePeriod(const KeplerianElements& Elements) noexcept
    {
        if ((IsValid(Elements) == true) && (IsClosed(Elements) == true)) 
        {
            return 2.0 * PI * Sqrt(Cube(Elements.SemiMajorAxis) / Elements.GravitationalParameter);
        }

        return Infinity<double>();        
    }

    /** 
     * @param Elements Orbital elements
     * @return Mean Radial Period (s)
     */
    constexpr double CalculateMeanRadialPeriod(const KeplerianElements& Elements) noexcept
    {
        // Elliptical    
        if (IsClosed(Elements) == true)
        {
            return Sqrt(Cube(Elements.SemiMajorAxis) / Elements.GravitationalParameter);
        }
        // Hyperbolic
        else if (IsHyperbolic(Elements) == true)
        {
            return Sqrt(-Cube(Elements.SemiMajorAxis) / Elements.GravitationalParameter);
        }
        else
        {
            return 2.0 * Sqrt(Cube(Elements.SemiParameter) / Elements.GravitationalParameter);
        }
    }    

    /** 
     * @param Elements Orbital elements 
     * @return Instantaneous Radius of the orbit (m), relative to the orbital barycentre
     */
    constexpr double CalculateRadius(const KeplerianElements& Elements) noexcept
    {
        return Elements.SemiParameter / (1.0 + Elements.Eccentricity * Cos(Elements.TrueAnomoly));
    }    

    /** 
     * Compute the keplerian orbital elements given a position and velocity state vector.
     * Assumes a two body problem
     * Based upon Algorithm 9 as detailed in "Fundamentals of Astrodynamics and Applications"
     * David A. Vallado, 4th Edition
     * 
     * @param Position instantaneous position (x, y, z) (m) of the orbiting satellite in the body centred frame
     * @param Velocity instantaneous velocity (x, y, z) (m/s) of the orbiting satellite in the body centred frame
     * @param GravitationalParameter newtonian gravitational parameter of the central body (m3 / s2)
     * 
     * @return KeplerianElements
     */
    constexpr KeplerianElements Newtonian2Kepler(const Vector3& Position, const Vector3& Velocity, double GravitationalParameter) noexcept
    {
        const auto Radius = Position.Norm();

        // Invalid orbit
        if ((Radius == 0.0) || (Velocity.NormSquared() == 0.0))
        {
            return KeplerianElements{};
        }

        KeplerianElements Result;   
        Result.GravitationalParameter = GravitationalParameter; 

        const auto SpeedSquared        = Velocity.NormSquared();
        const auto AngularMomentum     = Vector3::Cross(Position, Velocity);
        const auto AngularMomentumMagn = AngularMomentum.Norm();
        const auto NodeVector          = Vector3::Cross(Vector3::UNIT_Z(), AngularMomentum);
        const auto NodeVectorMagn      = NodeVector.Norm();
        const auto KinematicDot        = Vector3::Dot(Position, Velocity);
        const auto EccentricityVector  = ((SpeedSquared - GravitationalParameter / Radius) * Position 
            - KinematicDot * Velocity) / GravitationalParameter;
        const auto MechanicalEnergy    = 0.5 * SpeedSquared - GravitationalParameter / Radius;

        // Calculate eccentricity    
        Result.Eccentricity = EccentricityVector.Norm();

        // Calculate orbit magnitude
        if (Result.Eccentricity == 1.0) // Orbit is a parabola
        {
            Result.SemiParameter = AngularMomentumMagn * AngularMomentumMagn / GravitationalParameter;
            Result.SemiMajorAxis = Infinity<double>();     
        }
        else // Otherwise
        {
            Result.SemiMajorAxis = -GravitationalParameter / (2.0 * MechanicalEnergy);
            Result.SemiParameter = Result.SemiMajorAxis * (1.0 - Result.Eccentricity * Result.Eccentricity);
        }

        // Orbital inclination from equatorial plane
        Result.Inclination = Acos(AngularMomentum.Z / AngularMomentumMagn);

        // Right ascension of ascending node
        if (NodeVectorMagn > 0.0)
        {
            Result.Node = Acos(NodeVector.X / NodeVectorMagn);
            if (NodeVector.Y < 0.0)
            {
                Result.Node = 2.0 * PI - Result.Node;
            }

            // Argument of the perigee
            Result.ArgumentPerigee = Acos(Vector3::Dot(NodeVector, EccentricityVector) / (Result.Eccentricity * NodeVectorMagn));
            if (EccentricityVector.Z < 0.0)
            {
                Result.ArgumentPerigee = 2.0 * PI - Result.ArgumentPerigee;    
            }       

            // Special case parameter - circular inclined
            Result.ArgumentLatitude = Acos(Vector3::Dot(NodeVector, Position) / (NodeVectorMagn * Radius));
            if (Position.Z < 0.0)
            {
                Result.ArgumentLatitude = 2.0 * PI - Result.ArgumentLatitude;
            }         
        }

        if (Result.Eccentricity > 0.0)
        {
            // Position within the orbit
            Result.TrueAnomoly = Acos(Vector3::Dot(EccentricityVector, Position) / (Result.Eccentricity * Radius));
            if (KinematicDot < 0.0)
            {
                Result.TrueAnomoly = 2.0 * PI - Result.TrueAnomoly;    
            }
                
            // Special case parameter - elliptical equatorial            
            Result.TrueLongitudeOfPeriapsis = Acos(EccentricityVector.X / Result.Eccentricity);
            if (EccentricityVector.Y < 0.0)
            {
                Result.TrueLongitudeOfPeriapsis = 2.0 * PI - Result.TrueLongitudeOfPeriapsis;    
            }
        }        

        // Special case parameter - circular equatorial
        Result.TrueLongitude = Acos(Position.X / Radius);
        if (Position.Y < 0.0)
        {
            Result.TrueLongitude = 2.0 * PI - Result.TrueLongitude;    
        }

        return Result;    
    }

    /** 
     * Coverts keplerian orbital elements to position and velocity state vectors
     * Assumes a 2 body problem. Assumes an aberration free problem to compute light time
     * Based upon Algorithm 10 as detailed in "Fundamentals of Astrodynamics and applications"
     * David A. Vallado, 4th Edition
     * @param Elements keplerian orbital elements
     * @return EphemerisState (Position, Velocity, LightTime) 
     */
    constexpr EphemerisState Kepler2Newtonian(const KeplerianElements& Elements) noexcept
    {
        // Invalid Elements    
        if (IsValid(Elements) == false)
        {
            return EphemerisState{};
        }

        const auto Classification = ClassifyOrbit(Elements);

        double UseAnomoly = 0.0, UseNode = 0.0, UsePerigee = 0.0;
        if (Classification == OrbitClassification::CIRCULAR_EQUATORIAL)
        {
            UseAnomoly = Elements.TrueLongitude;
        }
        else if (Classification == OrbitClassification::CIRCULAR_INCLINED)
        {
            UseNode = Elements.Node;
            UseAnomoly = Elements.ArgumentLatitude;
        }
        else if (Classification == OrbitClassification::ELLIPTICAL_EQUATORIAL)
        {
            UsePerigee = Elements.TrueLongitudeOfPeriapsis;    
            UseAnomoly = Elements.TrueAnomoly;
        }
        else
        {
            UsePerigee = Elements.ArgumentPerigee;
            UseNode = Elements.Node;
            UseAnomoly = Elements.TrueAnomoly;
        }

        const auto CosAnomoly = Cos(UseAnomoly);
        const auto SinAnomoly = Sin(UseAnomoly);
        const auto Distance = Elements.SemiParameter / (1.0 + Elements.Eccentricity * CosAnomoly);
        const auto Coeff2 = Sqrt(Elements.GravitationalParameter / Elements.SemiParameter);

        // Newtonian state within the orbital plane
        const auto PosPQW = Vector3({Distance * CosAnomoly, Distance * SinAnomoly, 0.0});
        const auto VelPQW = Vector3({-Coeff2 * SinAnomoly, Coeff2 * (Elements.Eccentricity + CosAnomoly), 0.0});

        // Rotation from orbital plane to central body frame
        const auto Rot = 
            Quaternion::FromVectorAngle(Vector3::UNIT_Z(), -UsePerigee) *
            Quaternion::FromVectorAngle(Vector3::UNIT_X(), -Elements.Inclination) *
            Quaternion::FromVectorAngle(Vector3::UNIT_Z(), -UseNode);

        // Newtonian state
        return EphemerisState{.Pos = Rot.Rotate(PosPQW), .Vel = Rot.Rotate(VelPQW), .LightTime =  Distance / SPEED_LIGHT};
    }

    
    /** 
     * Computes the eccentric anomoly using the eccentricity and the true anomoly
     * Will return the parabolic or hyperbolic anomoly in the case of a non-elliptic orbit
     * @param TrueAnomoly True anomoly of the orbit (rad)
     * @param Eccentricity Eccentricity of the orbit
     * @return Eccentric anomoly (rad)
     */
    constexpr double TrueToEccentricAnomoly(double TrueAnomoly, double Eccentricity) noexcept
    {    
        if (Eccentricity < 1.0)    
        {
            // Elliptical    
            const double CosNu = Cos(TrueAnomoly);
            const double SinNu = Sin(TrueAnomoly);
            const double Denominator = (1.0 + Eccentricity * CosNu);
            const double SinE = SinNu * Sqrt(1.0 - Square(Eccentricity)) / Denominator;
            const double CosE = (Eccentricity + CosNu) / Denominator;
            return Atan2(SinE, CosE);
        }
        else if (Eccentricity == 1.0)
        {
            // Parabolic    
            return Tan(0.5 * TrueAnomoly);
        }
        else
        {
            // Hyperbolic    
            return Asinh(Sin(TrueAnomoly) * Sqrt(Square(Eccentricity) - 1.0) / (1.0 + Eccentricity * Cos(TrueAnomoly)));
        }
    }

    /** 
     * Computes the true anomoly using the eccentricity and eccentric/parabolic/hyperbolic anomoly.
     * @param EccentricAnomoly Eccentric anomoly (elliptic), hyperbolic anomoly (hyperbolic) or parabolic anomoly (parabolic) of the orbit (rad)
     * @param Eccentricity Eccentricity of the orbit
     * @return True anomoly (rad)
     */
    constexpr double EccentricToTrueAnomoly(double Anomoly, double Eccentricity) noexcept
    {    
        if (Eccentricity < 1.0)
        {
            // Elliptical    
            const double CosE = Cos(Anomoly);
            return Acos((CosE - Eccentricity) / (1.0 - Eccentricity * CosE));
        }
        else if (Eccentricity == 1.0)
        {
            // Parabolic    
            return 2.0 * Atan(Anomoly);
        }
        else
        {
            // Hyperbolic
            const double CoshH = Cosh(Anomoly);
            return Acos( (CoshH - Eccentricity) / (1.0 - Eccentricity * CoshH) );
        }
    }

    /** 
     * Calculates the mean anomoly of the orbit from the eccentric/hyperbolic/parabolic anomoly and the eccentricity
     * @param Anomoly eccentric/hyperbolic/parabolic anomoly
     * @param Eccentricity eccentricity of the orbit
     * @return Mean Anomoly
     */
    constexpr double EccentricToMeanAnomoly(double Anomoly, double Eccentricity) noexcept
    {
        // Elliptical    
        if (Eccentricity < 1.0)
        {
            return Anomoly - Eccentricity * Sin(Anomoly);
        }
        // Hyperbolic
        else if (Eccentricity > 1.0)
        {
            return Eccentricity * Sinh(Anomoly) - Anomoly;
        }
        // Parabolic
        else
        {
            return Anomoly + Cube(Anomoly) / 3.0;
        }
    }

    /** 
     * Computes the values of the C2, C3 coefficients
     * @return C2, C3 coefficients at the given angle
     */
    constexpr CCoefficents CalculateCoefficients(double Angle) noexcept
    {
        if (Angle > 1.0E-6)
        {
            const double SqrtAngle = Sqrt(Angle);
            return CCoefficents{.C2 = (1.0 - Cos(SqrtAngle)) / Angle, .C3 = (SqrtAngle - Sin(SqrtAngle)) / Cube(SqrtAngle)};
        }
        else if (Angle < -1.0E-6)
        {
            const double SqrtAngle = Sqrt(-Angle);    
            return CCoefficents{.C2 = (1.0 - Cosh(SqrtAngle)) / Angle, .C3 = (Sinh(SqrtAngle) - SqrtAngle) / Cube(SqrtAngle)};
        }
        else
        {
            // Taylor series approximation truncated to 3 terms O(Angle^2)
            return CCoefficents{.C2 = 0.5 - Angle / 24.0, .C3 = 1.0 / 6.0 - Angle / 120.0};
        }
    }    
}

