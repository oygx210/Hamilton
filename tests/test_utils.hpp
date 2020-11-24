#pragma once

#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "math/quaternion.hpp"

/**
 * Compile time "is near" comparison
 * 
 * @param X First comparative value 
 * @param Y Second comparative value
 * @param Error Comparative max absolute error
 * @return true if Abs(X - Y) < Error
 */
template <typename T>
constexpr bool IsNear(T X, T Y, T Error) noexcept
{
    if (Abs(X - Y) < Error) return true;
    return false;
}

/**
 * Element wise compile time "is near" comparison for vectors
 * 
 * @param V1 First comparative vector
 * @param V2 Second comparative vector
 * @param Error Maximum element wise error
 * @return true if each element of V1 - V2 satisfies |v1i - v2i| < Error
 */
constexpr bool IsVector3Near(const Vector3& V1, const Vector3& V2, double Error) noexcept
{
    return (IsNear(V1.X, V2.X, Error) && 
            IsNear(V1.Y, V2.Y, Error) && 
            IsNear(V1.Z, V2.Z, Error));
}

/**
 * Element wise compile time "is near" comparison for quaternions
 * 
 * @param Q1 First comparative quaternion
 * @param Q2 Second comparative quaternion
 * @param Error Maximum element wise error
 * @return true if each element of Q1 - Q2 satisfies |q1i - q2i| < Error
 */
constexpr bool IsQuaternionNear(const Quaternion& Q1, const Quaternion& Q2, double Error) noexcept
{
    return (IsNear(Q1.X, Q2.X, Error) && 
            IsNear(Q1.Y, Q2.Y, Error) && 
            IsNear(Q1.Z, Q2.Z, Error) &&
            IsNear(Q1.S, Q2.S, Error));
}

/**
 * Element wise compile time is near comparison for 3 x 3 matrices
 * @param M1 First comparative matrix
 * @param M2 Second comparative matrix
 * @param Error Maximum element wise error
 * @return true if each element of M1 - M2 satisfies |m1ij - m2ij| < Error
 */
constexpr bool IsMatrix3Near(const Matrix3& M1, const Matrix3& M2, double Error) noexcept
{
    return (IsNear(M1.XX, M2.XX, Error) && 
            IsNear(M1.XY, M2.XY, Error) && 
            IsNear(M1.XZ, M2.XZ, Error) &&
            IsNear(M1.YX, M2.YX, Error) && 
            IsNear(M1.YY, M2.YY, Error) && 
            IsNear(M1.YZ, M2.YZ, Error) &&
            IsNear(M1.ZX, M2.ZX, Error) && 
            IsNear(M1.ZY, M2.ZY, Error) && 
            IsNear(M1.ZZ, M2.ZZ, Error));
}