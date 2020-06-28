#pragma once

#include "core_math.hpp"
#include "axis3.hpp"

/*
 * Defines a three component vector in 3d space
 * Obeys standard vector operations and mathematics
 */
class Vector3 : public Axis3
{
public:
    /* 
     * Construct from components, call signature as:
     * Vector3 ({X, Y, Z}), or 
     * Vector3({.X = X, .Y = Y, .Z = Z}),  or
     * Vector3({}), zero vector
     */
    constexpr Vector3(const Axis3& In) : Axis3{In} { }

    /* Initialise as a zero vector */
    static constexpr Vector3 ZERO(void) 
    {
        return Vector3({0.0, 0.0, 0.0});
    }

    /* Unit Vector (1, 0, 0) */
    static constexpr Vector3 UNIT_X(void)	
    {
        return Vector3({1.0, 0.0, 0.0});
    }

    /* Unit Vector (0, 1, 0) */
    static constexpr Vector3 UNIT_Y(void)	
    {
        return Vector3({0.0, 1.0, 0.0});
    }

    /* Unit vector (0, 0, 1) */
    static constexpr Vector3 UNIT_Z(void)	
    {
        return Vector3({0.0, 0.0, 1.0});
    }

    /* Return the vector norm squared */
    constexpr double NormSquared(void) const
    {
        return X * X + Y * Y + Z * Z;
    }

    /* Return the norm */
    constexpr double Norm(void) const
    {
        return Sqrt(NormSquared());
    }

    /* Calculate cross product with another vector */
    constexpr Vector3 Cross(const Vector3& U) const
    {
        return Vector3({Y * U.Z - Z * U.Y, -X * U.Z + Z * U.X, X * U.Y - Y * U.X});
    }

    /* Calculate cross product of two vectors */
    constexpr static Vector3 Cross(const Vector3& U, const Vector3& V)
    {
        return Vector3({U.Y * V.Z - U.Z * V.Y, -U.X * V.Z + U.Z * V.X, U.X * V.Y - U.Y * V.X});
    }

    /* Calculate dot product with another vector*/
    constexpr double Dot(const Vector3& U) const
    {
        return X * U.X + Y * U.Y + Z * U.Z;
    }

    /* Calculate dot product of two vectors*/
    constexpr static double Dot(const Vector3& U, const Vector3& V)
    {
        return U.X * V.X + U.Y * V.Y + U.Z * V.Z;
    }

    /* Return the unit vector */
    constexpr Vector3 Unit(void) const
    {
        const auto Magn = Norm();	
        if (Magn > 0)	
        {
            return *this / Magn;
        }
        return ZERO();
    }

    /* Checks if the vector is a zero vector */
    constexpr bool IsZeroVector(void) const 
    {
        if (X != 0) return false;
        if (Y != 0) return false;
        if (Z != 0) return false;

        return true;        
    }

    // 
    // Vector operations
    //

    /* Vector negation */
    constexpr Vector3 operator-() const
    {
        return Vector3({-X, -Y, -Z});
    }

    /* Vector Addition */
    constexpr Vector3 operator+(const Vector3& V) const
    {
        return Vector3({X + V.X, Y + V.Y, Z + V.Z});
    }

    /* Vector subtraction*/
    constexpr Vector3 operator-(const Vector3& V) const
    {
        return Vector3({X - V.X, Y - V.Y, Z - V.Z});
    }

    /* Vector multiplication by scalar*/
    constexpr Vector3 operator*(double A) const
    {
        return Vector3({A * X, A * Y, A * Z});
    }

    /* Vector division by scalar*/
    constexpr Vector3 operator/(double A) const
    {
        return Vector3({X / A, Y / A, Z / A});
    }

    /* Vector in place addition*/
    void operator+=(const Vector3& V)
    {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
    }

    /* Vector in place subtraction */
    void operator-=(const Vector3& V)
    {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;
    }

    /* Vector in place multiplcation by scalar*/
    void operator*=(double A)
    {
        X *= A;
        Y *= A;
        Z *= A;
    }

    /* vector in place division by scalar */
    void operator/=(double A)
    {
        X /= A;
        Y /= A;
        Z /= A;
    }

    /* Vector equality comparison */
    constexpr bool operator==(const Vector3& V) const
    {
        if (X != V.X) return false;
        if (Y != V.Y) return false;
        if (Z != V.Z) return false;
        return true;
    }

    /* Vector not equal comparison */
    constexpr bool operator!=(const Vector3& V) const
    {
        return !(*this == V);
    }
};

/* Vector left multiply by scalar */
constexpr Vector3 operator*(double A, const Vector3& Rhs)
{
    return Rhs * A;
}
