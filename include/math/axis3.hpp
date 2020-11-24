#pragma once

#include "utils/hstring.hpp"

/**
 * Object which is defined by 3 independent elements
 *
 * Is not a vector, has no concept of a magnitude and does not obey vector operations
 */
class Axis3
{
public:

    // Element access
    double X = 0.0;
    double Y = 0.0;
    double Z = 0.0;

    /** 
     * @return Vector(0, 0, 0)
     */
    constexpr static Axis3 ZERO(void) noexcept
    {
        return Axis3{0.0, 0.0, 0.0};
    }
    
    /** 
     * @return String representation
     */	
    HString ToString(void) const
    {
        return "(" + HString{X} + ", " + HString{Y} + ", " + HString{Z} + ")";
    }
};