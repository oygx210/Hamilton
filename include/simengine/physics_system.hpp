#pragma once

#include "meta/indexable.hpp"

/** 
 * Manager class for the physics of the system
 */
class PhysicsSystem
{
public:

    /** 
     * Advance the state of the physics system
     * @param Time new time in reference epoch (s)
     */
    void Update(double Time);

    // const Indexable& GetComponent()    

private:
};