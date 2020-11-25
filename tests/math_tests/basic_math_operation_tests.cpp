#include "gtest/gtest.h"
#include "test_utils.hpp"
#include "math/core_math.hpp"

// Test some templated basic operations
TEST(Math, BasicOperations)
{
    // Min    
    {
        static_assert(Min(-1.0, -2.0) == -2.0);
        static_assert(Min(0.5, -1.0, 2.4) == -1.0);
    }    

    // Max
    {
        static_assert(Max(-11.0, 1.0) == 1.0);
        static_assert(Max(3.0, -4.0, 5.0) == 5.0);
    }   

    // AbsMin    
    {
        static_assert(AbsMin(-1.0, -2.0) == 1.0);
        static_assert(AbsMin(0.5, -1.0, 2.4, -0.3) == 0.3);
    }    

    // AbsMax
    {
        static_assert(AbsMax(-11.0, 1.0) == 11.0);
        static_assert(AbsMax(3.0, -7.0, 5.0) == 7.0);
    }        

    // Sum
    {
        static_assert(Sum(3.0, 4.0) == 7.0);
        static_assert(Sum(1.0, 5.0, 7.0) == 13.0);
    }    

    // Average
    {
        static_assert(Average(3.0, 1.0) == 2.0);
        static_assert(Average(1.0, 2.0, 3.0) == 2.0);
    }   

    // Clamp
    {
        static_assert(Clamp(3.0, 2.0, 4.0) == 3.0);
        static_assert(Clamp(5.0, 2.0, 4.0) == 4.0);
        static_assert(Clamp(1.0, 2.0, 4.0) == 2.0);
    }     
}