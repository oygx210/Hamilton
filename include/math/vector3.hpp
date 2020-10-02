#pragma once

#include "core_math.hpp"
#include "axis3.hpp"

/**
 * Defines a three component vector in 3d space
 * Obeys standard vector operations and mathematics
 */
class Vector3 : public Axis3
{
public:
    /** 
     * Construct from components, call signature as:
     * Vector3 ({X, Y, Z}), or 
     * Vector3({.X = X, .Y = Y, .Z = Z}),  or
     * Vector3({}), zero vector
     * @param In Axis3 components of the vector
     */
    constexpr Vector3(const Axis3& In) noexcept: Axis3{In} { }

    /** 
     * @return Vector(0, 0, 0) 
     */
    static constexpr Vector3 ZERO(void) noexcept
    {
        return Vector3({0.0, 0.0, 0.0});
    }

    /**
     * @return Unit Vector (1, 0, 0) 
     */
    static constexpr Vector3 UNIT_X(void) noexcept
    {
        return Vector3({1.0, 0.0, 0.0});
    }

    /** 
     * @return Unit Vector (0, 1, 0) 
     */
    static constexpr Vector3 UNIT_Y(void) noexcept
    {
        return Vector3({0.0, 1.0, 0.0});
    }

    /** 
     * @return Unit vector (0, 0, 1) 
     */
    static constexpr Vector3 UNIT_Z(void) noexcept
    {
        return Vector3({0.0, 0.0, 1.0});
    }

    /**
     * @return Vector Norm squared 
     */
    constexpr double NormSquared(void) const noexcept
    {
        return X * X + Y * Y + Z * Z;
    }

    /** 
     * @return Vector Norm 
     */
    constexpr double Norm(void) const noexcept
    {
        return Sqrt(NormSquared());
    }

    /**
     * Calculate vector cross product 
     * @param U Other ector to cross product with
     * @return Cross product this x U
     */
    constexpr Vector3 Cross(const Vector3& U) const noexcept
    {
        return Vector3({Y * U.Z - Z * U.Y, -X * U.Z + Z * U.X, X * U.Y - Y * U.X});
    }

    /** 
     * Calculate vector cross product
     * @param U first vector
     * @param V second vector
     * @return Cross Product U x V
     */
    constexpr static Vector3 Cross(const Vector3& U, const Vector3& V) noexcept
    {
        return Vector3({U.Y * V.Z - U.Z * V.Y, -U.X * V.Z + U.Z * V.X, U.X * V.Y - U.Y * V.X});
    }

    /**
     * Calculate vector dot product
     * @param U Other vector to dot product with
     * @return Dot Product this . U
     */
    constexpr double Dot(const Vector3& U) const noexcept
    {
        return X * U.X + Y * U.Y + Z * U.Z;
    }

    /**
     * Calculate vector dot product
     * @param U first vector
     * @param V second vector
     * @return U . V
     */
    constexpr static double Dot(const Vector3& U, const Vector3& V) noexcept
    {
        return U.X * V.X + U.Y * V.Y + U.Z * V.Z;
    }

    /** 
     * @return Unit vector of `this` 
     */
    constexpr Vector3 Unit(void) const noexcept
    {
        const auto MagnSq = NormSquared();	
        if (MagnSq > 0)	
        {
            if (MagnSq == 1.0)
            {
                return *this;    
            }

            return *this / Sqrt(MagnSq);
        }
        return ZERO();
    }

    /**
     * @return `true` if this vector is a zero vector 
     */
    constexpr bool IsZeroVector(void) const noexcept
    {
        return (*this == Vector3::ZERO());
    }

    // 
    // Vector operations
    //

    /** Vector negation */
    constexpr Vector3 operator-() const noexcept
    {
        return Vector3({-X, -Y, -Z});
    }

    /** Vector Addition */
    constexpr Vector3 operator+(const Vector3& V) const noexcept
    {
        return Vector3({X + V.X, Y + V.Y, Z + V.Z});
    }

    /** Vector subtraction*/
    constexpr Vector3 operator-(const Vector3& V) const noexcept
    {
        return Vector3({X - V.X, Y - V.Y, Z - V.Z});
    }

    /** Vector multiplication by scalar*/
    constexpr Vector3 operator*(double A) const noexcept
    {
        return Vector3({A * X, A * Y, A * Z});
    }

    /** Vector division by scalar*/
    constexpr Vector3 operator/(double A) const noexcept
    {
        return Vector3({X / A, Y / A, Z / A});
    }

    /** Vector in place addition*/
    void operator+=(const Vector3& V) noexcept
    {
        X += V.X;
        Y += V.Y;
        Z += V.Z;
    }

    /** Vector in place subtraction */
    void operator-=(const Vector3& V) noexcept
    {
        X -= V.X;
        Y -= V.Y;
        Z -= V.Z;
    }

    /** Vector in place multiplcation by scalar*/
    void operator*=(double A) noexcept
    {
        X *= A;
        Y *= A;
        Z *= A;
    }

    /** vector in place division by scalar */
    void operator/=(double A) noexcept
    {
        X /= A;
        Y /= A;
        Z /= A;
    }

    /** Vector equality comparison */
    constexpr bool operator==(const Vector3& V) const noexcept
    {
        if (X != V.X) return false;
        if (Y != V.Y) return false;
        if (Z != V.Z) return false;
        return true;
    }

    /** Vector not equal comparison */
    constexpr bool operator!=(const Vector3& V) const noexcept
    {
        return !(*this == V);
    }
};

/** Vector left multiply by scalar */
constexpr Vector3 operator*(double A, const Vector3& Rhs) noexcept
{
    return Rhs * A;
}
