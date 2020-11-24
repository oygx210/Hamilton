#include "gtest/gtest.h"

#include "utils/harray.hpp"

/** 
 * @file array_tests.cpp
 * Tests the custom HArray type
 */

TEST(Array, Functionality)
{
    // Basics    
    {
        // Default constructor    
        HArray Arr;

        ASSERT_TRUE(Arr.IsEmpty());

        // Reserve elements but do not resize
        Arr.Reserve(2);
        ASSERT_TRUE(Arr.IsEmpty());
        ASSERT_EQ(Arr.Size(), 0);

        // Add some elements
        Arr.EmplaceBack(3.0);
        Arr.EmplaceBack(5.0);

        ASSERT_FALSE(Arr.IsEmpty());

        // Legnth check
        ASSERT_EQ(Arr.Size(), 2);

        // Retrieve element
        ASSERT_EQ(Arr[0], 3.0);
        ASSERT_EQ(Arr[1], 5.0);
        ASSERT_EQ(Arr.Front(), 3.0);
        ASSERT_EQ(Arr.Back(), 5.0);

        // Re-assign element
        Arr[1] = 7.0;
        ASSERT_EQ(Arr[1], 7.0);

        // Resize array
        Arr.Resize(3);
        ASSERT_EQ(Arr.Size(), 3);

        Arr.Back() = -1.0;
        Arr.Front() = 0.5;
        ASSERT_EQ(Arr.Back(), -1.0);
        ASSERT_EQ(Arr.Front(), 0.5);

        // Element wise operations
        Arr[0] += 0.5;
        Arr[2] -= 1.0;

        ASSERT_EQ(Arr[0], 1.0);
        ASSERT_EQ(Arr[2], -2.0);

        Arr[1] *= 2.0;
        ASSERT_EQ(Arr[1], 14.0);

        Arr[1] /= 2.0;
        ASSERT_EQ(Arr[1], 7.0);

        // Clear array
        Arr.Clear();
        ASSERT_EQ(Arr.Size(), 0);
    }    

    // Alternative constructors
    {
        // Number elements and initial value    
        auto Arr = HArray<int>::OfSizeWithInit(3, -1);
        ASSERT_EQ(Arr.Size(), 3);
        ASSERT_EQ(Arr[0], -1);
        ASSERT_EQ(Arr[1], -1);
        ASSERT_EQ(Arr[2], -1);

        // Templated initialiser list
        HArray<float> Arr2 {1.0f, 2.0f, 3.0f, 4.0f};
        ASSERT_EQ(Arr2.Size(), 4);
        ASSERT_EQ(Arr2[0], 1.0f);
        ASSERT_EQ(Arr2[1], 2.0f);
        ASSERT_EQ(Arr2[2], 3.0f);
        ASSERT_EQ(Arr2[3], 4.0f);
    }

    // Array math operations
    {
        HArray Arr1, Arr2, Arr3, Arr4;

        Arr1.Reserve(10);
        Arr2.Reserve(10);
        Arr3.Reserve(10);
        Arr4.Reserve(10);

        // Populate
        for (int Index = 0; Index < 10; Index++)
        {
            Arr1.EmplaceBack(static_cast<double>(Index + Index * 3));
            Arr2.EmplaceBack(static_cast<double>(Index - Index * 3));
            Arr3.EmplaceBack(static_cast<double>(Index));
            Arr4.EmplaceBack(static_cast<double>(3 * Index));
        }

        ASSERT_EQ(Arr1, Arr3 + Arr4);
        ASSERT_EQ(Arr2, Arr3 - Arr4);

        Arr1 -= Arr4;
        ASSERT_EQ(Arr1, Arr3);

        Arr2 += Arr4;
        ASSERT_EQ(Arr2, Arr3);

        Arr3 *= 3.0;
        ASSERT_EQ(Arr3, Arr4);

        Arr4 /= 3.0;
        ASSERT_EQ(Arr4, Arr1);
    }    

    // Insertion and removal
    {
        HArray Arr {0.0, 1.0, 2.0, 3.0};
        Arr.Insert(Arr.Begin() + 1, 5.0);
        ASSERT_EQ(Arr, HArray(0.0, 5.0, 1.0, 2.0, 3.0));

        Arr.Erase(Arr.Begin() + 2);
        ASSERT_EQ(Arr, HArray(0.0, 5.0, 2.0, 3.0));        

        Arr.Erase(Arr.Begin() + 1, Arr.Begin() + 3);
        ASSERT_EQ(Arr, HArray(0.0, 3.0));

        HArray Arr2 {4.0, 5.0, 6.0};
        Arr2.Insert(Arr2.End(), Arr.Begin(), Arr.End());
        Arr2.AppendBack(Arr);
        ASSERT_EQ(Arr2, HArray(4.0, 5.0, 6.0, 0.0, 3.0, 0.0, 3.0));
    }  
}