#pragma once

#include "math/core_math.hpp"
#include "math/vector3.hpp"
#include "math/quaternion.hpp"

#include <string>

/* 
 * Compile time is near comparison
 * 
 */
template <typename T>
constexpr bool IsNear(T X, T Y, T Error)
{
    if (Fabs(X - Y) < Error) return true;
    return false;
}

/* 
 * Element wise compile time is near comparison for vectors
 */
constexpr bool IsVector3Near(const Vector3& V1, const Vector3& V2, double Error)
{
    return (IsNear(V1.X, V2.X, Error) && 
            IsNear(V1.Y, V2.Y, Error) && 
            IsNear(V1.Z, V2.Z, Error));
}

/* 
 * Element wise compile time is near comparison for quaternions
 */
constexpr bool IsQuaternionNear(const Quaternion& Q1, const Quaternion& Q2, double Error)
{
    return (IsNear(Q1.X, Q2.X, Error) && 
            IsNear(Q1.Y, Q2.Y, Error) && 
            IsNear(Q1.Z, Q2.Z, Error) &&
            IsNear(Q1.S, Q2.S, Error));
}

/* 
 * Element wise compile time is near comparison for 3 x 3 matrices
 */
constexpr bool IsMatrix3Near(const Matrix3& M1, const Matrix3& M2, double Error)
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