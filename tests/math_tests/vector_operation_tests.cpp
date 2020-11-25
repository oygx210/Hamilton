#include "gtest/gtest.h"
#include "math/vector3.hpp"

TEST(Axis3, Default)
{
    // Static tests
    static_assert(Axis3{}.X == 0.0);
    static_assert(Axis3{}.Y == 0.0);
    static_assert(Axis3{}.Z == 0.0);

    static_assert(Axis3::ZERO().X == 0.0);
    static_assert(Axis3::ZERO().Y == 0.0);
    static_assert(Axis3::ZERO().Z == 0.0);
}

TEST(Vector, BasicOperations)
{
    {
        static_assert(Vector3({}) == Vector3::ZERO());
        static_assert(Vector3::ZERO() == Vector3({0.0, 0.0, 0.0}));
        static_assert(Vector3::UNIT_X() == Vector3({1.0, 0.0, 0.0}));
        static_assert(Vector3::UNIT_Y() == Vector3({0.0, 1.0, 0.0}));
        static_assert(Vector3::UNIT_Z() == Vector3({0.0, 0.0, 1.0}));
    }   

    // Creation and  Comparison
    {    
        // Construction    
        constexpr auto V1 = Vector3({.X = 1.0, .Y=-1.0, .Z=2.0});
        static_assert(V1.X == 1.0);
        static_assert(V1.Y == -1.0);
        static_assert(V1.Z == 2.0);

        // Copy and equality
        constexpr auto V2 = V1;        
        static_assert(V2 == V1);

        // move assignment    
        constexpr auto V3 = Vector3({.X=0.0, .Y=4.0, .Z=-1.0});
        static_assert(V3.X == 0.0);
        static_assert(V3.Y == 4.0);
        static_assert(V3.Z == -1.0);

        // Not equals
        static_assert(V3 != V1);

        static_assert(V1.IsZeroVector() == false);
        static_assert(Vector3({}).IsZeroVector());

        constexpr auto V4 = -V1;
        static_assert(V4.X == -V1.X);
        static_assert(V4.Y == -V1.Y);
        static_assert(V4.Z == -V1.Z);
    }

    // Operations with scalar    
    {
        constexpr auto V1 = Vector3({.X = 1.0, .Y=-1.0, .Z=2.0});

        constexpr auto V2 = V1 * 4.0;
        static_assert(V2 == 4.0 * V1);

        constexpr auto V3 = V1 / 4.0;
        static_assert(V3 == V1 / 4.0);

        auto V4 = V1;
        V4 *= 4.0;
        ASSERT_EQ(V4.X, V1.X * 4.0);
        ASSERT_EQ(V4.Y, V1.Y * 4.0);
        ASSERT_EQ(V4.Z, V1.Z * 4.0);

        V4 = V1;
        V4 /= 4.0;        
        ASSERT_EQ(V4.X, V1.X / 4.0);
        ASSERT_EQ(V4.Y, V1.Y / 4.0);
        ASSERT_EQ(V4.Z, V1.Z / 4.0);        
    }    
    
    // Vector - Vector operations
    {
        constexpr auto V1 = Vector3({.X=0.0, .Y=2.0, .Z=3.0});
        constexpr auto V2 = Vector3({.X=3.0, .Y=-2.0, .Z= 1.0});
        auto V3 = V1;
        auto V4 = V2;

        // Vector addition
        static_assert(V2 + V1 == Vector3({.X=3.0, .Y=0.0, .Z=4.0}));

        // Vector subtraction
        static_assert(V2 - V1 == Vector3({.X=3.0, .Y=-4.0, .Z=-2.0}));
        
        // In place addition        
        V3 += V2;
        ASSERT_EQ(V3, Vector3({.X=3.0, .Y=0.0, .Z=4.0}));

        // In place subtraction
        V4 -= V1;
        ASSERT_EQ(V4, Vector3({.X=3.0, .Y=-4.0, .Z=-2.0}));

        // dot product
        static_assert(Vector3::Dot(V1, V2) == -1.0);
        static_assert(V1.Dot(V2) == -1.0);

        // cross product
        static_assert(Vector3::Cross(V1, V2) == Vector3({8.0, 9.0, -6.0}));
        static_assert(V1.Cross(V2) == Vector3({8.0, 9.0, -6.0}));
    }    

    // Magnitude and scaling
    {
        constexpr auto V1 = Vector3({.X = 4.0, .Y = 2.0, .Z = -3.0});
        static_assert(V1.NormSquared() == 29.0);
        static_assert(V1.Norm() == Sqrt(29.0));
        static_assert(V1.Unit() == V1 / V1.Norm());
    }    
}