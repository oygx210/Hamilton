#pragma once

#include "utils/meta.hpp"
#include "gcem.hpp"
#include <cmath>
#include <type_traits>

/* 
 * Defines the math functionality used, defaults to the standard library implementation
 * but allows certain calls to be overriden where required
 */

/* PI */
constexpr double PI = 3.14159265358979323846;

/*
 * Convert degrees to radians
 */
inline constexpr double D2R(double Degrees) 
{
    return PI * Degrees / 180.0;
}

/* 
 * Convert radians to degrees
 */
inline constexpr double R2D(double Radians) 
{
    return 180.0 * Radians / PI;
}

/*
 * compile time definition of fabs
 */
inline constexpr double Fabs(double Val)
{
    if (Val >= 0) return Val;
    return -Val;
}

/*
 * Signum function
 */
template <typename T> 
inline constexpr T Signum(T Val) 
{
    if (Val < 0) return T{-1};
    return T{1};
}

/* 
 * Square root
 */
template <typename T>
inline constexpr T Sqrt(T Val)
{
    if (std::is_constant_evaluated() == true)
    {
        return gcem::sqrt(Val);
    }
    else
    {
        return sqrt(Val);
    }
}

/* 
 * Sin
 */
template <typename T>
inline constexpr T Sin(T Val)
{
    if (std::is_constant_evaluated() == true)  
    {
        return gcem::sin(Val);
    }
    else
    {
        return sin(Val);
    }
}

/* 
 * Cos
 */
template <typename T>
inline constexpr T Cos(T Val)
{
    if (std::is_constant_evaluated() == true)    
    {
        return gcem::cos(Val);
    }
    else
    {
        return cos(Val);
    }
}

// Note: gcem::atan implementation performs an implicit 
// long double to double conversion, this selectivly 
// ignores the thrown warning
DISABLE_WARNING_PUSH    
DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA
/* 
 * Arctangent functionality
 */
template <typename T>
inline constexpr T Atan2(T Y, T X)
{
    if (std::is_constant_evaluated() == true)    
    {
        return gcem::atan2(Y, X);
    }
    else
    {
        return atan2(Y, X);
    }
}
DISABLE_WARNING_POP

/* 
 * Arcsin function
 */
template <typename T>
inline constexpr T Asin(T Val)
{
    if (std::is_constant_evaluated() == true)    
    {
        return gcem::asin(Val);
    }
    else
    {
        return asin(Val);
    }
}

/* 
 * Power function
 * 
 * NOTE: GCEM performs implicit double to T conversion
 * Must selectivly ignore
 */
DISABLE_WARNING_PUSH    
DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA    
template <typename T, typename S>
inline constexpr T Pow(T Val, S Expn)
{
    if (std::is_constant_evaluated() == true)    
    {
        
        return gcem::pow(Val, Expn);
    }
    else
    {
        return pow(Val, Expn);
    }
}
DISABLE_WARNING_POP

/*
 * float modulus
 */
template <typename T>
inline constexpr T Fmod(T Val, T Divisor)
{
    if (std::is_constant_evaluated() == true)    
    {
        return gcem::fmod(Val, Divisor);
    }
    else
    {
        return fmod(Val, Divisor);
    }
}