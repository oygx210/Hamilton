#pragma once

#include <string>

/*
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

    /* Initialise with all zero components */
    constexpr static Axis3 ZERO(void) noexcept
    {
        return Axis3 {0.0, 0.0, 0.0};
    }
    
    /* Get a string representation of the axis type */	
    std::string ToString(void) const noexcept
    {
        return "(" + std::to_string(X) + ", " 
                   + std::to_string(Y) + ", " 
                   + std::to_string(Z) + ")";
    }
};