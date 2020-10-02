#pragma once

#include "vector3.hpp"
#include "matrix3.hpp"
#include "quaternion.hpp"

/**
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

    /**
     * Default rotator is the unit quaternion
     */
    constexpr Rotator() noexcept : 
        mQuat{Quaternion::IDENTITY()}, 
        mDCM{Matrix3::IDENTITY()}
    {

    }
    
    /**
     * Initialise from a quaternion
     * @param Quat Quaternion to initialise this rotator with
     */
    constexpr Rotator(const Quaternion& Quat) noexcept : 
        mQuat{Quat.Unit()}, 
        mDCM{mQuat.DirectCosineMatrix()} 
    {
      
    }

    /** 
     * @return Vector x component of the quaternion 
     */
    constexpr double X(void) const noexcept {return mQuat.X;}

    /**
     * @return Vector y component of the quaternion
     */
    constexpr double Y(void) const noexcept {return mQuat.Y;}

    /**
     * @return Vector z component of the quaternion
     */
    constexpr double Z(void) const noexcept {return mQuat.Z;}

    /** 
     * @return Scalar s component of the quaternion    
     */
    constexpr double S(void) const noexcept {return mQuat.S;}

    /**
     * @return Unit vector (1, 0, 0) in the frame rotated into by the transform
     */
    constexpr Vector3 UnitX(void) const noexcept 
    {
        return Vector3({mDCM.XX, mDCM.YX, mDCM.ZX});
    }

    /** 
      * @return Unit vector (0, 1, 0) in the frame rotated into by the transform
      */
    constexpr Vector3 UnitY(void) const noexcept 
    {
        return Vector3({mDCM.XY, mDCM.YY, mDCM.ZY});
    }

    /** 
     * @return Unit vector (0, 0, 1) in the frame rotated into by the transform
     */
    constexpr Vector3 UnitZ(void) const noexcept
    {
        return Vector3({mDCM.XZ, mDCM.YZ, mDCM.ZZ});
    }

    /**
     * Get the euler angle (roll, pitch, yaw) representation of the quaternion 
     * describing the transformation between two frames.
     * 
     * Returns the rotations (counterclockwise) around the x, y, and z axis respectively
     * 
     * Assumes Body 3-2-1 sequence, i.e the resulting angles are determined by the sequence of: 
     * 
     * 3: Yaw about z axis
     * 2: Pitch about y axis
     * 1: Roll about x axis
     * 
     * @return Rotations (counterclockwise) around the x, y, and z axis respectively 
     */	
    constexpr Axis3 EulerAngles(void) const noexcept {return mQuat.EulerAngles();}

    /**
     * Rotate a vector using the cached rotation matrix
     * @param U Vector to be rotated
     * @return Vector `U` rotated by this
     */
    constexpr Vector3 Rotate(const Vector3& U) const noexcept {return mDCM * U;}

    /** 
     * Rotates a vector implicitly using the inverse quaternion. Note, if this
     * operation is required 3 or more times for the same rotator it is more efficient
     * to instantiate another rotator from the inverse of `this`
     * @param U Vector to be inversely rotated
     * @return Vector `U` rotated inversely by this
     */
    constexpr Vector3 RotateInv(const Vector3& U) const noexcept
    {
        return mQuat.RotateInv(U);
    }    

    /**
     * @return Underlying unit quaternion
     */
    constexpr const Quaternion& AsQuaternion(void) const noexcept {return mQuat;}

    /**
     * @return Direct cosine matrix representation of the quaternion 
     */
    constexpr const Matrix3& DirectCosineMatrix(void) const noexcept {return mDCM;}

    /**
     * Explicit initialiser to generate a quaternion which describes the minimum 
     * co-ordinate transformation between two frames O -> M by using a vector with
     * components known in both frames. Follows a right handed co-ordinate frame.
     * 
     * @param U Reference vector represented in frame O
     * @param V Reference vector represented in frame M
     * @return Rotator from frame O->M
     */
    static constexpr Rotator FromVectorPair(const Vector3& U, const Vector3& V) noexcept
    {
        return Rotator{Quaternion::FromVectorPair(U, V)};
    }

    /**
     * Explicit initialiser to generate a quaternion which describes the rotation
     * of a co-ordinate frame about an eigenaxis
     * 
     * @param U Eigenaxis to rotate frame about
     * @param Angle Counterclockwise rotation in radians of the frame about the eigenaxis
     * @return Rotator describing a rotation `Angle` about the vector `U`
     */
    static constexpr Rotator FromVectorAngle(const Vector3& U, double Angle) noexcept
    {
        return Rotator{Quaternion::FromVectorAngle(U, Angle)};
    }

    /** 
     * @return Inverse rotator 
     */
    constexpr Rotator Inverse(void) const noexcept
    {
        return Rotator({.X = -X(), .Y = -Y(), .Z = -Z(), .S = S()});
    }

    /**
     * @return Identity rotator (no rotation)
     */
    static constexpr Rotator IDENTITY(void) noexcept
    {
        return Rotator(Quaternion::IDENTITY(), Matrix3::IDENTITY());
    }

    /**
     * Compose a set of rotators together, performs only a single direct cosine matrix calculation
     * @param Rotations sequence of Rotators to compose together
     * @return Net Rotator
     */
    template <typename... Args>
    static constexpr Rotator Compose(const Args&... Rotations) noexcept
    {
        return Rotator((... * Rotations.AsQuaternion()));
    }

    /** Equality Comparison */	
    constexpr bool operator==(const Rotator& Rot) const noexcept
    {
        return (AsQuaternion() == Rot.AsQuaternion());
    }

    /** Inequality comparison */
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