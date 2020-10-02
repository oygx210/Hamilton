#pragma once

#include "kepler.hpp"

namespace TwoBody
{
    /** 
     * Kepler equations coefficients
     */
    struct CCoefficents
    {
        double C2 = 0.0;
        double C3 = 0.0;
    };

    /** 
     * Objects which stores and mutates an orbital state (as keplerian elements)    
     */
    class Orbit
    {
    public:    

        /** 
         * Instantiate from Keplerian elements
         * @param Elements Keplerian Elements
         * @return Orbit
         */
        static constexpr Orbit FromKeplerianElements(const KeplerianElements& Elements) noexcept
        {
            return Orbit(Elements);
        }

        /** 
         * Instantiate from Newtonian state
         * @param Position (x, y, z) (m)
         * @param Velocity (x, y ,z) (m/s)
         * @param GravitationalParameter Central body gravitational parameter (m3/s2)
         * @return Orbit
         */
        static constexpr Orbit FromNewtonian(const Vector3& Position, const Vector3& Velocity, double GravitationalParameter) noexcept
        {
            return Orbit(Position, Velocity, GravitationalParameter);
        }

        /** 
         * @return KeplerianElements
         */
        const KeplerianElements& GetElements(void) const noexcept {return mElements;}

        /** 
         * Computes the values of the C2, C3 coefficients
         * @return C2, C3 coefficients at the given angle
         */
        static constexpr CCoefficents CalculateCoefficients(double Angle) noexcept
        {
            if (Angle > 1.0E-6)
            {
                const double SqrtAngle = Sqrt(Angle);
                return CCoefficents{.C2 = (1.0 - Cos(SqrtAngle)) / Angle, .C3 = (SqrtAngle - Sin(SqrtAngle)) / Cube(SqrtAngle)};
            }
            else if (Angle < -1.0E-6)
            {
                double SqrtAngle = Sqrt(-Angle);    
                return CCoefficents{.C2 = (1.0 - Cosh(SqrtAngle)) / Angle, .C3 = (Sinh(SqrtAngle) - SqrtAngle) / Cube(SqrtAngle)};
            }
            else
            {
                // Taylor series approximation truncated to 3 terms O(Angle^2)
                return CCoefficents{.C2 = 0.5 - Angle / 24.0, .C3 = 1.0 / 6.0 - Angle / 120.0};
            }
        }

        /** 
         * Computes the eccentric anomoly using the eccentricity and the true anomoly
         * Will return the parabolic or hyperbolic anomoly in the case of a non-elliptic orbit
         * @param TrueAnomoly True anomoly of the orbit (rad)
         * @param Eccentricity Eccentricity of the orbit
         * @return Eccentric anomoly (rad)
         */
        static constexpr double TrueToEccentricAnomoly(double TrueAnomoly, double Eccentricity) noexcept
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
                return Tan(0.5 * TrueAnomoly);
            }
            else if (Eccentricity > 1.0)
            {
                return Asinh(Sin(TrueAnomoly) * Sqrt(Square(Eccentricity) - 1.0) / (1.0 + Eccentricity * Cos(TrueAnomoly)));
            }
        }

        /** 
         * Computes the true anomoly using the eccentricity and eccentric anomoly
         * @param EccentricAnomoly Eccentric anomoly of the orbit (rad)
         * @param Eccentricity Eccentricity of the orbit
         * @return True anomoly (rad)
         */
        static constexpr double EccentricToTrueAnomoly(double EccentricAnomoly, double Eccentricity) noexcept
        {    
            // Elliptical    
            const double CosE = Cos(EccentricAnomoly);
            return Acos((CosE - Eccentricity) / (1.0 - Eccentricity * CosE));
        }        

    private:

        // Current keplerian state
        KeplerianElements mElements{};

        // rad
        double EccentricAnomoly = 0.0;

        // rad
        double MeanAnomoly = 0.0f;             

        constexpr Orbit(const KeplerianElements& Elements) noexcept : mElements{Elements}
        {

        }


        constexpr Orbit(const Vector3& Position, const Vector3& Velocity, double GravitationalParameter) : 
            mElements{Newtonian2Kepler(Position, Velocity, GravitationalParameter)}
        {

        }
    };
}    