#pragma once

#include "math/core_math.hpp"
#include "vector3.hpp"
#include "matrix3.hpp"

#include <string>

/*
 * Quaternion Object
 * 
 * Initialise from components using c++20 designated initiliser syntax i.e
 * 
 * auto Q = Quaternion {.X = X, .Y = Y, .Z = Z, .S = S};
 * 
 * Or (not recommended) Q = Quaternion(X, Y, Z, S);
 * 
 * Can also initialise from two vectors:
 * 
 * auto Q = Quaternion::FromVectorPair(U, V);
 * 
 * Or from a vector-angle pair:
 * 
 * auto Q = Quaternion::FromVectorAngle(U, Theta);
 *
 * Or as an identity/zero quaternion
 * 
 * auto Q = Quaternion::IDENTITY();
 * auto Q = Quaternion::ZERO();
 * 
 */
class Quaternion
{
public:

    // Quaternion elements
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;
    double S = 1.0;

    /*
     * Explicit initialiser to generate a quaternion which describes the minimum 
     * co-ordinate transformation between two frames O -> M by using a vector with
     * components known in both frames. Follows a right handed co-ordinate frame.
     * 
     * Inputs:
     * Vector3 U, reference vector represented in frame O
     * Vector3 V, reference vector represented in frame M
     */
    static constexpr Quaternion FromVectorPair(const Vector3& U, const Vector3& V) noexcept
    {
        // if one or more input vectors is a zero vector, then use a zero quaternion
        if ((V.IsZeroVector() == true) || (U.IsZeroVector() == true))
        {
            return ZERO();
        }

        const auto Vc = V.Cross(U);
        const auto Vd = V.Dot(U);

        // Vectors are parallel
        if (Vc.NormSquared() == 0.0)
        {
            if (Vd >= 0)
            {
                return IDENTITY();
            }
            else if ((V.Y == 0) && (V.Z == 0))
            {
                return Quaternion{.X = 0.0, .Y = 0.0, .Z = 1.0, .S = 0.0};
            }
            else
            {
                const auto Magn = Sqrt(V.Y * V.Y + V.Z * V.Z);
                return Quaternion{.X = 0, .Y = -V.Y / Magn, .Z = V.Z / Magn, .S = 0.0};
            }
        }

        const auto SV = Sqrt(U.NormSquared() * V.NormSquared()) + Vd;

        return Quaternion{.X = Vc.X, .Y = Vc.Y, .Z = Vc.Z, .S = SV}.Unit();
    }

    /* 
     * Explicit initialiser to generate a quaternion which describes the rotation
     * of a co-ordinate frame about an eigenaxis
     * 
     * Inputs:
     * Vector3 U, eigenaxis to rotate frame about
     * double Angle, counterclockwise rotation in radians of the frame about the eigenaxis
     */
    static constexpr Quaternion FromVectorAngle(const Vector3& U, double Angle) noexcept
    {
        const auto SAngle = Sin(Angle * 0.5);
        const auto Unit = U.Unit();
        return Quaternion{.X = Unit.X * SAngle,
                          .Y = Unit.Y * SAngle,
                          .Z = Unit.Z * SAngle,
                          .S = Cos(Angle * 0.5)};
    }

    /*
     * Explicit initialiser for an identity quaternion
     */
    static constexpr Quaternion IDENTITY(void) noexcept
    {
        return Quaternion{.X=0.0, .Y=0.0, .Z=0.0, .S=1.0};
    }

    /* Returns a zero quaternion */	
    static constexpr Quaternion ZERO(void) noexcept
    {
        return Quaternion{.X = 0.0, .Y = 0.0, .Z = 0.0, .S = 0.0};
    }

    /* Return the unit normalised unit quaternion */
    constexpr Quaternion Unit(void) const noexcept
    {
        const auto Magn = Norm();

        if (Magn > 0)
        {
            return Quaternion {.X = X / Magn, 
                               .Y = Y / Magn, 
                               .Z = Z / Magn, 
                               .S = S / Magn};
        }
        return ZERO();
    }

    /* Rotate a vector using the quaternion*/
    constexpr Vector3 Rotate(const Vector3& U) const noexcept
    {
        const auto Tx = Z * U.Y - Y * U.Z;
        const auto Ty = X * U.Z - Z * U.X;
        const auto Tz = Y * U.X - X * U.Y;

        return Vector3({U.X + 2.0 * (Tx * S + Ty * Z - Tz * Y),
                        U.Y + 2.0 * (Ty * S + Tz * X - Tx * Z),
                        U.Z + 2.0 * (Tz * S + Tx * Y - Ty * X)});
    }

    /* Rotate a vector using the implicit inverse quaternion */
    constexpr Vector3 RotateInv(const Vector3& U) const noexcept
    {
        const auto Tx = -Z * U.Y + Y * U.Z;
        const auto Ty = -X * U.Z + Z * U.X;
        const auto Tz = -Y * U.X + X * U.Y;

        return Vector3({U.X + 2.0 * (Tx * S - Ty * Z + Tz * Y),
                        U.Y + 2.0 * (Ty * S - Tz * X + Tx * Z),
                        U.Z + 2.0 * (Tz * S - Tx * Y + Ty * X)});
    }

    /* Return the inverse of the quaternion */
    constexpr Quaternion Inverse(void) const noexcept
    {
        const auto Magn = Norm();
        if (Magn > 0)
        {
            return Quaternion{.X = -X / Magn, .Y = -Y / Magn, .Z = -Z / Magn, .S = S / Magn};
        }
        else
        {
            return ZERO();
        }
    }

    /* Return the direct cosine matrix representation of the quaternion */
    constexpr Matrix3 DirectCosineMatrix(void) const noexcept
    {
        const auto QX2 = X * X;
        const auto QXY = X * Y;
        const auto QXZ = X * Z;
        const auto QXS = X * S;
        const auto QY2 = Y * Y;
        const auto QYZ = Y * Z;
        const auto QYS = Y * S;
        const auto QZ2 = Z * Z;
        const auto QZS = Z * S;

        return Matrix3{
            .XX = 1.0 - 2.0 * (QY2 + QZ2),
            .XY = 2.0 * (QXY + QZS),
            .XZ = 2.0 * (QXZ - QYS),
            .YX = 2.0 * (QXY - QZS),
            .YY = 1.0 - 2.0 * (QX2 + QZ2),
            .YZ = 2.0 * (QYZ + QXS),
            .ZX = 2.0 * (QXZ + QYS),
            .ZY = 2.0 * (QYZ - QXS),
            .ZZ = 1.0 - 2.0 * (QX2 + QY2)} / NormSquared();
    }

    /* Return the norm of the quaternion */
    constexpr double Norm(void) const noexcept
    {
        return Sqrt(NormSquared());
    }

    /* Return the square of the quaternion norm. More efficient for size comparisons than norm */
    constexpr double NormSquared(void) const noexcept
    {
        return S * S + X * X + Y * Y + Z * Z;
    }

    /* 
     * Return the derivative of the quaternion 
     * Inputs:
     * Omega: rotational rates around each axis
     */
    constexpr Quaternion Derivative(const Axis3& Omega) const noexcept
    {
        return Quaternion{.X =  0.5 * (Omega.X * S + Omega.Y * Z - Omega.Z * Y),
                          .Y =  0.5 * (Omega.Y * S + Omega.Z * X - Omega.X * Z),
                          .Z =  0.5 * (Omega.Z * S + Omega.X * Y - Omega.Y * X),
                          .S = -0.5 * (Omega.X * X + Omega.Y * Y + Omega.Z * Z)};
    }
    
    //
    // Operations
    //

    /* Multiplication with quaternion (quaternion composition) */
    constexpr Quaternion operator*(const Quaternion& Q) const noexcept
    {
        return Quaternion{.X = Q.X * S + Q.S * X + Q.Z * Y - Q.Y * Z,
                          .Y = Q.Y * S + Q.S * Y - Q.Z * X + Q.X * Z,
                          .Z = Q.Z * S + Q.S * Z + Q.Y * X - Q.X * Y,
                          .S = Q.S * S - Q.X * X - Q.Y * Y - Q.Z * Z};
    }

    /* Addition with another quaternion */
    constexpr Quaternion operator+(const Quaternion& Q)	const noexcept
    {
        return Quaternion{.X = X + Q.X,
                          .Y = Y + Q.Y,
                          .Z = Z + Q.Z,
                          .S = S + Q.S};
    }

    /* Subtraction with another quaternion */
    constexpr Quaternion operator-(const Quaternion& Q)	const noexcept
    {
        return Quaternion{.X = X - Q.X,
                          .Y = Y - Q.Y,
                          .Z = Z - Q.Z,
                          .S = S - Q.S};
    }	

    /* Negation */
    constexpr Quaternion operator-() const noexcept
    {
        return Quaternion{.X = -X, .Y = -Y, .Z=-Z, .S=-S};
    }

    /* In place addition with another quaternion */
    void operator+=(const Quaternion& Q) noexcept
    {
        X += Q.X,
        Y += Q.Y;
        Z += Q.Z;
        S += Q.S;
    }	

    /* In place subtraction with another quaternion */
    void operator-=(const Quaternion& Q) noexcept
    {
        X -= Q.X,
        Y -= Q.Y;
        Z -= Q.Z;
        S -= Q.S;
    }	

    /* Multiplication by scalar */
    constexpr Quaternion operator*(double A) const noexcept
    {
        return Quaternion{.X = X * A,
                          .Y = Y * A,
                          .Z = Z * A,
                          .S = S * A};
    }		

    /* Division by scalar */
    constexpr Quaternion operator/(double A) const noexcept
    {
        return Quaternion{.X = X / A,
                          .Y = Y / A,
                          .Z = Z / A,
                          .S = S / A};
    }		

    /* Equality Comparison */	
    constexpr bool operator==(const Quaternion& Q) const noexcept
    {
        if (X != Q.X) return false;
        if (Y != Q.Y) return false;
        if (Z != Q.Z) return false;
        if (S != Q.S) return false;

        return true;    
    }

    /* Inequality comparison */
    constexpr bool operator!=(const Quaternion& Q) const noexcept
    {
        return !(*this == Q);
    }

    /* 
     * Get the euler angle (roll, pitch, yaw) representation of the quaternion 
     * describing the transformation between two frames.
     * 
     * Returns the rotations (counterclockwise) around the x, y, and z axis respectively
     * 
     * Assumes Body 3-2-1 sequence, i.e the resulting angles are determined by the sequence of
     * 3: Yaw about z axis
     * 2: Pitch about y axis
     * 1: Roll about x axis
     */	
    constexpr Axis3 EulerAngles() const noexcept
    {
        const auto NormSq = NormSquared();

        // Cant get the euler angles of a zero quaternion
        if (NormSq == 0)
        {
            return Axis3::ZERO();
        }

        // Gimbal lock check
        const auto SinPitch = 2.0 * (S * Y - Z * X) / NormSq;

        // North pole gimbal lock
        if (SinPitch >= 1.0)		
        {
            return Axis3{0.0, 0.5 * PI, 2.0 * Atan2(X, S)};
        }
        // south pole gimbal lock
        else if (SinPitch <= -1.0)
        {
            return Axis3{0.0, -0.5 * PI, -2.0 * Atan2(X, S)};
        }
        // Not gimbal locked
        else
        {
            return Axis3{    
                Atan2(2.0 * (S * X + Y * Z), 1.0 - 2.0 * (X * X + Y * Y)),
                Asin(SinPitch),
                Atan2(2.0 * (S * Z + X * Y), 1.0 - 2.0 * (Y * Y + Z * Z))};
        }
    }

    /* String representation of the quaternion */
    std::string	ToString() const noexcept
    {
        return "(" + std::to_string(X) + ", " 
                   + std::to_string(Y) + ", " 
                   + std::to_string(Z) + ", " 
                   + std::to_string(S) + ")";
    }
};

/* Quaternion left multiply by scalar */
constexpr Quaternion operator*(double A, const Quaternion& Rhs) noexcept
{
    return Rhs * A;
}
