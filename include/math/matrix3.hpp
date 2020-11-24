#pragma once

#include "axis3.hpp"
#include "vector3.hpp"

/**
 * General 3 x 3 matrix object
 */
class Matrix3
{
public:
    
    /**
     * @return Identity Matrix 
     */
    static constexpr Matrix3 IDENTITY(void) noexcept
    {
        return Matrix3{.XX = 1.0, .YY = 1.0, .ZZ = 1.0};
    }

    /**
     * @return Zero Matrix 
     */
    static constexpr Matrix3 ZERO(void) noexcept
    {
        return Matrix3{.XX = 0.0, .XY = 0.0, .XZ = 0.0, 
                       .YX = 0.0, .YY = 0.0, .YZ = 0.0, 
                       .ZX = 0.0, .ZY = 0.0, .ZZ = 0.0};
    }
    
    //	
    // Elements
    //
    double XX = 0.0;
    double XY = 0.0;
    double XZ = 0.0;
    double YX = 0.0;
    double YY = 0.0;
    double YZ = 0.0;
    double ZX = 0.0;
    double ZY = 0.0;
    double ZZ = 0.0;
    
    /** 
     * @return Determinant of `this` 
     */	
    constexpr double Determinant(void) const noexcept
    {
        return XX * (YY * ZZ - YZ * ZY) 
              - XY * (YX * ZZ - YZ * ZX) 
             + XZ * (YX * ZY - YY * ZX);
    }

    /** 
     * @return Transpose of `this` 
     */
    constexpr Matrix3 Transpose(void) const noexcept
    {
        return Matrix3{.XX = XX, .XY = YX, .XZ = ZX, 
                          .YX = XY, .YY = YY, .YZ = ZY, 
                          .ZX = XZ, .ZY = YZ, .ZZ = ZZ}; 
    }

    /** 
     * Outer product of two vectors 
     * @param U First vector
     * @param V Second vector
     * @return The matrix `U`^T x `V`
     */
    constexpr static Matrix3 Outer(const Vector3& U, const Vector3& V) noexcept
    {
        return Matrix3{ .XX = U.X * V.X, .XY = U.X * V.Y, .XZ = U.X * V.Z,
                        .YX = U.Y * V.X, .YY = U.Y * V.Y, .YZ = U.Y * V.Z,
                        .ZX = U.Z * V.X, .ZY = U.Z * V.Y, .ZZ = U.Z * V.Z };
    }

    // template <typename T>
    // constexpr static T Solve(const Matrix3& A, const T& B) noexcept
    // {
    //     T Result = T::ZERO();
    //     Matrix3 Intermediate = A;

    //     for (const size_t K = 0; I < 3; ++I)
    //     {
    //         // Find Pivot

    //         // Check if Matrix is singular or ill posed            

    //         // Do all rows below pivot
    //         for (const size_t I = K + 1; I < 3; ++I)
    //         {
    //             // Calculate all intermediate values on and above diagonal

    //             // Fill lower triangular matrix with zeros                
    //         }
    //     }

    //     // Do back substitution
    // }

    /* Calculate eigenvalues and eigenvectors */
    // EigenResult eigenValuesVectors(void) const;

    // 
    // Matrix Operations
    //	

    /** Multiplication by matrix */
    constexpr Matrix3 operator*(const Matrix3& Mat) const noexcept
    {
        return Matrix3{
            .XX = XX * Mat.XX + XY * Mat.YX + XZ * Mat.ZX,
            .XY = XX * Mat.XY + XY * Mat.YY + XZ * Mat.ZY,
            .XZ = XX * Mat.XZ + XY * Mat.YZ + XZ * Mat.ZZ,
            .YX = YX * Mat.XX + YY * Mat.YX + YZ * Mat.ZX,
            .YY = YX * Mat.XY + YY * Mat.YY + YZ * Mat.ZY,
            .YZ = YX * Mat.XZ + YY * Mat.YZ + YZ * Mat.ZZ,
            .ZX = ZX * Mat.XX + ZY * Mat.YX + ZZ * Mat.ZX,
            .ZY = ZX * Mat.XY + ZY * Mat.YY + ZZ * Mat.ZY,
            .ZZ = ZX * Mat.XZ + ZY * Mat.YZ + ZZ * Mat.ZZ};
    }

    /** Multiplication by axis */
    constexpr Axis3 operator*(const Axis3& Vect) const noexcept
    {
        return Axis3{.X = XX * Vect.X + XY * Vect.Y + XZ * Vect.Z,
                     .Y = YX * Vect.X + YY * Vect.Y + YZ * Vect.Z,
                     .Z = ZX * Vect.X + ZY * Vect.Y + ZZ * Vect.Z};
    }

    /** Multiplication by vector */
    constexpr Vector3 operator*(const Vector3& Vect) const noexcept
    {
        return Vector3({.X = XX * Vect.X + XY * Vect.Y + XZ * Vect.Z,
                        .Y = YX * Vect.X + YY * Vect.Y + YZ * Vect.Z,
                        .Z = ZX * Vect.X + ZY * Vect.Y + ZZ * Vect.Z});
    }	

    /** Multiplication by scalar */
    constexpr Matrix3 operator*(double A) const noexcept
    {
        return Matrix3{.XX = A * XX, .XY = A * XY, .XZ = A * XZ, 
                       .YX = A * YX, .YY = A * YY, .YZ = A * YZ, 
                       .ZX = A * ZX, .ZY = A * ZY, .ZZ = A * ZZ};
    }

    /** Division by Scalar */
    constexpr Matrix3 operator/(double A) const noexcept
    {
        return Matrix3{.XX = XX / A, .XY = XY / A, .XZ = XZ / A, 
                       .YX = YX / A, .YY = YY / A, .YZ = YZ / A, 
                       .ZX = ZX / A, .ZY = ZY / A, .ZZ = ZZ / A};
    }

    /** Addition with matrix */
    constexpr Matrix3 operator+(const Matrix3& Mat) const noexcept
    {
        return Matrix3{.XX = XX + Mat.XX, .XY = XY + Mat.XY, .XZ = XZ + Mat.XZ, 
                       .YX = YX + Mat.YX, .YY = YY + Mat.YY, .YZ = YZ + Mat.YZ, 
                       .ZX = ZX + Mat.ZX, .ZY = ZY + Mat.ZY, .ZZ = ZZ + Mat.ZZ};
    }

    /** Substraction with matrix */
    constexpr Matrix3 operator-(const Matrix3& Mat) const noexcept
    {
        return Matrix3{.XX = XX - Mat.XX, .XY = XY - Mat.XY, .XZ = XZ - Mat.XZ, 
                       .YX = YX - Mat.YX, .YY = YY - Mat.YY, .YZ = YZ - Mat.YZ, 
                       .ZX = ZX - Mat.ZX, .ZY = ZY - Mat.ZY, .ZZ = ZZ - Mat.ZZ};
    }
    
    /** Negation */
    constexpr Matrix3 operator-(void) const noexcept
    {
        return Matrix3{.XX = -XX, .XY = -XY, .XZ = -XZ, 
                       .YX = -YX, .YY = -YY, .YZ = -YZ, 
                       .ZX = -ZX, .ZY = -ZY, .ZZ = -ZZ};
    }

    /** Equality comparison */
    constexpr bool operator==(const Matrix3& Mat) const noexcept
    {
        return ((XX == Mat.XX) && (XY == Mat.XY) && (XZ == Mat.XZ) &&
                (YX == Mat.YX) && (YY == Mat.YY) && (YZ == Mat.YZ) &&
                (ZX == Mat.ZX) && (ZY == Mat.ZY) && (ZZ == Mat.ZZ));
    }

    /** Inequality comparison */		
    constexpr bool operator!=(const Matrix3& Mat) const noexcept
    {
        return !(*this == Mat);
    }

    /**
     * @return String representation of Matrix 
     */
    HString ToString() const noexcept
    {
        return "[" + HString{XX} + ", " 
                   + HString{XY} + ", " 
                   + HString{XZ} + "; " 
                   + HString{YX} + ", " 
                   + HString{YY} + ", " 
                   + HString{YZ} + "; " 
                   + HString{ZX} + ", " 
                   + HString{ZY} + ", " 
                   + HString{ZZ} + "]";
    }
};

/** Multiplication by scalar on left*/
constexpr Matrix3 operator*(double A, const Matrix3& Mat) noexcept
{
    return Mat * A;
}
