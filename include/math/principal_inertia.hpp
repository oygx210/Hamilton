#pragma once

#include "math/axis3.hpp"
#include "math/vector3.hpp"
#include "math/matrix3.hpp"

/** 
 * Struct which contains the result of a principal inertia's and axis calculations
 */
struct PrincipalInertia
{
    /// Principal inertias (kg.m2)    
    Axis3 PrincipalInertias{};

    /// Principal unit x axis
    Vector3 UnitX = Vector3::UNIT_X();

    /// Principal unit y axis    
    Vector3 UnitY = Vector3::UNIT_Y();

    /// Principal unit z axis    
    Vector3 UnitZ = Vector3::UNIT_Z();

    /// Flag for a successful calculation. Will always succeed unless the 
    /// inertia tensor is non-physical
    bool ValidInertiaTensor = true;    
};

/** 
 * Calculates the principal inertias, and the axis along which they act in the body frame.
 * @param InertiaTensor Inertia Tensor of the object in the body frame
 * @return PrincipalInerta result
 */
inline constexpr PrincipalInertia CalculatePrincipalInertia(const Matrix3& InertiaTensor)
{
    // TODO    
    return PrincipalInertia{};
}