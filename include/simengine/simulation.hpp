#pragma once

#include "physics_system.hpp"

/** 
 * Manager class for a simulation instance
 * 
 * WIP
 */
class Simulation
{
public:

    /** 
     * Advance simulation state by one tick
     */
    void Tick(void);

    /** 
     * Get a constant reference to the physics system
     */
    const PhysicsSystem& GetPhysicsSystem(void) const {return mPhysicsSystem;}

private:

    /// Physics system
    PhysicsSystem mPhysicsSystem{};
};