#pragma once

#include "vector3.hpp"
#include "matrix3.hpp"
#include "quaternion.hpp"

/* 
 * Quaternion based object which caches the value of the direct cosine
 * 
 * More efficient than raw Quaternion representation for 3 or more
 * vector rotations of the same quaternion state.
 *
 * Will always encode a unit quaternion, i.e Norm = 1, unless a zero
 * quaternion is provided, in which case the norm will always be 0
 * 
 * Since the dcm must be recalculated for any component change, the individual
 * components cannot be reassigned (unlike with quaternions)
 */
class Rotator
{
public:

    // Default rotator is the unit quaternion
    constexpr Rotator() noexcept : 
        mQuat{Quaternion::IDENTITY()}, 
        mDCM{Matrix3::IDENTITY()}
    {

    }
    
    // Initialise from a quaternion
    constexpr Rotator(const Quaternion& Quat) noexcept : 
        mQuat{Quat.Unit()}, 
        mDCM{mQuat.DirectCosineMatrix()} 
    {

    }

    // vector x component of the quaternion
    constexpr double X(void) const noexcept {return mQuat.X;}

    // vector y component of the quaternion
    constexpr double Y(void) const noexcept {return mQuat.Y;}

    // Vector z component of the quaternion
    constexpr double Z(void) const noexcept {return mQuat.Z;}

    // Scalar component of the quaternion    
    constexpr double S(void) const noexcept {return mQuat.S;}

    // Unit vector (1, 0, 0) in the frame rotated into by the transform
    constexpr Vector3 UnitX(void) const noexcept 
    {
        return Vector3({mDCM.XX, mDCM.YX, mDCM.ZX});
    }

    // Unit vector (0, 1, 0) in the frame rotated into by the transform
    constexpr Vector3 UnitY(void) const noexcept 
    {
        return Vector3({mDCM.XY, mDCM.YY, mDCM.ZY});
    }

    // Unit vector (0, 0, 1) in the frame rotated into by the transform
    constexpr Vector3 UnitZ(void) const noexcept
    {
        return Vector3({mDCM.XZ, mDCM.YZ, mDCM.ZZ});
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
    constexpr Axis3 EulerAngles(void) const noexcept {return mQuat.EulerAngles();}

    // Rotate a vector using the cached rotation matrix
    constexpr Vector3 Rotate(const Vector3& U) const noexcept {return mDCM * U;}

    // Rotate a vector implicitly using the inverse quaternion
    constexpr Vector3 RotateInv(const Vector3& U) const noexcept
    {
        return mQuat.RotateInv(U);
    }    

    // Get the quaternion representation
    constexpr const Quaternion& AsQuaternion(void) const noexcept {return mQuat;}

    /* 
     * Return the direct cosine matrix representation of the quaternion 
     */
    constexpr const Matrix3& DirectCosineMatrix(void) const noexcept {return mDCM;}

    /*
     * Explicit initialiser to generate a quaternion which describes the minimum 
     * co-ordinate transformation between two frames O -> M by using a vector with
     * components known in both frames. Follows a right handed co-ordinate frame.
     * 
     * Inputs:
     * Vector3 U, reference vector represented in frame O
     * Vector3 V, reference vector represented in frame M
     */
    static constexpr Rotator FromVectorPair(const Vector3& U, const Vector3& V) noexcept
    {
        return Rotator{Quaternion::FromVectorPair(U, V)};
    }

    /* 
     * Explicit initialiser to generate a quaternion which describes the rotation
     * of a co-ordinate frame about an eigenaxis
     * 
     * Inputs:
     * Vector3 U, eigenaxis to rotate frame about
     * double Angle, counterclockwise rotation in radians of the frame about the eigenaxis
     */
    static constexpr Rotator FromVectorAngle(const Vector3& U, double Angle) noexcept
    {
        return Rotator{Quaternion::FromVectorAngle(U, Angle)};
    }

    /* Return the inverse rotator */
    constexpr Rotator Inverse(void) const noexcept
    {
        return Rotator({.X = -X(), .Y = -Y(), .Z = -Z(), .S = S()});
    }

    /*
     * Explicit initialiser for an identity quaternion
     */
    static constexpr Rotator IDENTITY(void) noexcept
    {
        return Rotator(Quaternion::IDENTITY(), Matrix3::IDENTITY());
    }

    /*
     * Compose a set of rotators together, performs only a single direct cosine matrix calculation
     */
    template <typename... Args>
    static constexpr Rotator Compose(const Args&... Rotations) noexcept
    {
        return Rotator((... * Rotations.AsQuaternion()));
    }

    /* Equality Comparison */	
    constexpr bool operator==(const Rotator& Rot) const noexcept
    {
        return (AsQuaternion() == Rot.AsQuaternion());
    }

    /* Inequality comparison */
    constexpr bool operator!=(const Rotator& Rot) const noexcept
    {
        return (AsQuaternion() != Rot.AsQuaternion());
    }    

private:

    // Initialise using an already computed direct cosine matrix
    constexpr Rotator(const Quaternion& Quat, const Matrix3& DCM) noexcept :
        mQuat{Quat}, 
        mDCM{DCM}
    {

    }

    // Quaternion representation
    Quaternion mQuat = Quaternion::IDENTITY();

    // Direct cosine matrix    
    Matrix3 mDCM = Matrix3::IDENTITY();
};