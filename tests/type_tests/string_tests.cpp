#include "gtest/gtest.h"

#include "utils/hstring.hpp"

/** 
 * @file string_tests.cpp
 * Tests the custom HString type
 */

TEST(String, Functionality)
{
    {
        // Default String    
        HString Str{};
        ASSERT_EQ(Str, "");
        ASSERT_EQ(Str.Size(), 0);
        ASSERT_EQ(Str.Front(), '\0');
        ASSERT_EQ(Str.Back(), '\0');

        // Reassign
        Str = "Now I have data";
        ASSERT_EQ(Str, "Now I have data");
        ASSERT_NE(Str, "Now I have gold");
        ASSERT_EQ(Str.Size(), 15);
        ASSERT_EQ(Str.Front(), 'N');
        ASSERT_EQ(Str.Back(), 'a');

        // Contains and finds
        ASSERT_TRUE(Str.Contains("data"));
        ASSERT_FALSE(Str.Contains("gold"));

        ASSERT_EQ(Str.Find("I"), 4);
        ASSERT_EQ(Str.Find("a"), 7);
        ASSERT_EQ(Str.Find("a", 8), 12);

        // Startswith, Endswith
        ASSERT_TRUE(Str.StartsWith("Now"));
        ASSERT_TRUE(Str.EndsWith("data"));
        ASSERT_FALSE(Str.StartsWith("How"));
        ASSERT_FALSE(Str.EndsWith("gold"));
        
        // Element access
        ASSERT_EQ(Str.At(4), 'I');
        ASSERT_EQ(Str.At(Str.Size()), '\0');
        ASSERT_EQ(Str.At(Str.Size() + 1), '\0');        

        Str.Clear();       
        ASSERT_EQ(Str, "");
        ASSERT_EQ(Str.Size(), 0);
    }

    {
        // Alternate constructor    
        HString Str = "A String";
        HString Str2 = Str;

        ASSERT_EQ(Str, Str2);

        // Substring
        ASSERT_EQ(Str.Substring(2, 0), "");
        ASSERT_EQ(Str.Substring(2, 1), "S");
        ASSERT_EQ(Str.Substring(2, 5), "Strin");
        ASSERT_EQ(Str.Substring(2, 6), "String");
        ASSERT_EQ(Str.Substring(2, 6), "String");        

        // Split
        ASSERT_EQ(Str.Split(" "), HArray<HString>("A", "String"));
        ASSERT_EQ(Str.Split("+"), HArray<HString>("A String"));
        ASSERT_EQ(HString{"how the turntables"}.Split("the"),
            HArray<HString>("how ", " turntables"));
        
        auto Splt = HString{"-a-poorly-formatted-string--"}.Split("-");
        ASSERT_EQ(Splt, HArray<HString>("", "a", "poorly", "formatted", "string", "", ""));

        // Join
        ASSERT_EQ(HString::Join(" ", HArray<HString>("A", "String")), Str);
        ASSERT_EQ(HString::Join("-+", HArray<HString>("", "", "data", "data", "")),
            "-+-+data-+data-+");
    }

    {
        // Replace
        HString Str = "Boogey Boogey Boogey";
        ASSERT_EQ(Str.Replace("Boogey", "Hey"), HString{"Hey Boogey Boogey"});
        ASSERT_EQ(Str.Replace("Boogey", "Hey", 1), HString{"Boogey Hey Boogey"});
        ASSERT_EQ(Str.Replace("Boogey", "Hey", Str.Size()), Str);
        ASSERT_EQ(Str.Replace("A", "O"), Str);

        // Replace All
        ASSERT_EQ(Str.ReplaceAll("Boogey", "Hey"), HString{"Hey Hey Hey"});
    }

    {
        // Append
        HString Str{"Hello"};
        HString Str2{" World"};
        HString Str3 = Str + " World";

        ASSERT_EQ(Str + Str2, HString{"Hello World"});
        ASSERT_EQ(Str3, HString{"Hello World"});
    }    

    {
        // Strip    
    }    
}