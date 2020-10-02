#pragma once

#include "utils/meta.hpp"
#include "gcem.hpp"
#include <cmath>
#include <type_traits>

/**
 * @file core_math.hpp
 * Defines the math functionality used, defaults to the standard library implementation
 * but allows certain calls to be overriden where required
 */

namespace Math
{
    /** PI */
    constexpr double PI = 3.14159265358979323846;

    /**
     * Convert degrees to radians
     * @param Degrees Angle in degrees
     * @return Angle in radians
     */
    inline constexpr double D2R(double Degrees) noexcept
    {
        return PI * Degrees / 180.0;
    }

    /**
     * Convert radians to degrees
     * @param Radians Angle in radians
     * @return Angle in degrees
     */
    inline constexpr double R2D(double Radians) noexcept
    {
        return 180.0 * Radians / PI;
    }

    /**
     * @return Absolute value of `Val`
     */
    template <typename T>
    inline constexpr T Fabs(T Val) noexcept
    {
        if (Val >= 0) return Val;
        return -Val;
    }

    /**
     * @return Sign of `Val`
     */
    template <typename T> 
    inline constexpr T Signum(T Val) noexcept
    {
        if      (Val <  0) return T{-1};
        else if (Val == 0) return T{0};
        else               return T{1};
    }

    /**
     * @return Square root of `Val`
     */
    template <typename T>
    inline constexpr T Sqrt(T Val) noexcept
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

    /** 
     * @return Trigonometric Sine of `Val`
     */
    template <typename T>
    inline constexpr T Sin(T Val) noexcept
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

    /**
     * @return Trigonometric Cosine of `Val`
     */
    template <typename T>
    inline constexpr T Cos(T Val) noexcept
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

    /** 
     * @return Trigonometric Tan of `Val`
     */
    template <typename T>
    inline constexpr T Tan(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)    
        {
            return gcem::tan(Val);
        }
        else
        {
            return tan(Val);
        }
    }

    // Note: gcem::atan implementation performs an implicit 
    // long double to double conversion, this selectivly 
    // ignores the thrown warning
    DISABLE_WARNING_PUSH    
    DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA
    /**
     * Preserves quadrant information of the input point (`X`, `Y`)
     * @return Arctangent of `Y`/`X`
     */
    template <typename T>
    inline constexpr T Atan2(T Y, T X) noexcept
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

    // Turn warnings back on
    DISABLE_WARNING_POP

    /**
     * @return Arcsine of `Val`
     */
    template <typename T>
    inline constexpr T Asin(T Val) noexcept
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

    /**
     * @return Arccosine of `Val`
     */
    template <typename T>
    inline constexpr T Acos(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)    
        {
            return gcem::acos(Val);
        }
        else
        {
            return acos(Val);
        }
    }

    // NOTE: GCEM performs implicit double to T conversion
    // Must selectivly ignore
    DISABLE_WARNING_PUSH    
    DISABLE_WARNING_TYPE_CONVERSION_POSSIBLE_LOSS_OF_DATA    

    /**
     * Power function
     * 
     * Prefer Square and Cube functionality for simple integer powers
     * 
     * @return `Val` ^ `Expn` 
     */
    template <typename T, typename S>
    inline constexpr T Pow(T Val, S Expn) noexcept
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

    // Turn warninings back on
    DISABLE_WARNING_POP

    /**
     * @return e^`Val`
     */
    template <typename T>
    inline constexpr T Exp(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::exp(Val);
        }
        else
        {
            return exp(Val);
        }
    }

    /** 
     * @return cosh(`Val`)
     */
    template <typename T>
    inline constexpr T Cosh(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::cosh(Val);
        }
        else
        {
            return cosh(Val);
        }
    }

    /** 
     * @return sinh(`Val`)
     */
    template <typename T>
    inline constexpr T Sinh(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::sinh(Val);
        }
        else
        {
            return sinh(Val);
        }
    }

    /** 
     * @return asinh(`Val`)
     */
    template <typename T>
    inline constexpr T Asinh(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::asinh(Val);
        }
        else
        {
            return asinh(Val);
        }
    }

    /** 
     * @return acosh(`Val`)
     */
    template <typename T>
    inline constexpr T Acosh(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::acosh(Val);
        }
        else
        {
            return acosh(Val);
        }
    }

    /** 
     * @return atanh(`Val`)
     */
    template <typename T>
    inline constexpr T Atanh(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::atanh(Val);
        }
        else
        {
            return atanh(Val);
        }
    }

    /**
     * Float modulo
     * @return `Val` mod `Divisor`
     */
    template <typename T>
    inline constexpr T Fmod(T Val, T Divisor) noexcept
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

    /**
     * @return `Val`^2
     */
    template <typename T>
    inline constexpr T Square(T Val) noexcept
    {
        return Val * Val;    
    }

    /**
     * @return `Val`^3
     */
    template <typename T>
    inline constexpr T Cube(T Val) noexcept
    {
        return Val * Val * Val;    
    }

    /**
     * @return `Val`^4
     */
    template <typename T>
    inline constexpr T Quart(T Val) noexcept
    {
        return Square(Square(Val));
    }
}

using namespace Math;