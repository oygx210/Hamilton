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
    inline constexpr T Abs(T Val) noexcept
    {
        if (Val >= T{0}) return Val;
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
     * @return Cube root of `Val`
     */
    template <typename T>
    inline constexpr T Cbrt(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::pow(Val, 1.0 / 3.0);
        }
        else
        {
            return cbrt(Val);
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
     * @return Arctangent of `X`
     */
    template <typename T>
    inline constexpr T Atan(T X) noexcept
    {
        if (std::is_constant_evaluated() == true)    
        {
            return gcem::atan(X);
        }
        else
        {
            return atan(X);
        }
    }

    // Turn warnings back on
    DISABLE_WARNING_POP    

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
     * Floor Function
     * @return Floor (Val)
     */
    template <typename T>
    inline constexpr T Floor(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::floor(Val);
        }
        else
        {
            return floor(Val);
        }
    }

    /** 
     * Ceiling Function
     * @return Ceiling (Val)
     */
    template <typename T>
    inline constexpr T Ceil(T Val) noexcept
    {
        if (std::is_constant_evaluated() == true)
        {
            return gcem::ceil(Val);
        }
        else
        {
            return ceil(Val);
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

    /**
     * @param X First comparison value
     * @param Y Second comparison value
     * @return Minimum value of X and Y
     */
    template <typename T>
    inline constexpr T Min(T X, T Y) noexcept
    {
        return (X <= Y) ? X : Y;
    }     

    /**
     * @param Head First comparison value
     * @param Comparators Remaining Comparison values
     * @return Minimum value
     */
    template <typename T, typename... Tail>
    inline constexpr T Min(T Head, Tail... Comparators) noexcept
    {
        return Min(Head, Min(Comparators...));
    }    

    /**
     * @param X First comparison value
     * @param Y Second comparison value
     * @return Maximum value of X and Y
     */
    template <typename T>
    inline constexpr T Max(T X, T Y) noexcept
    {
        return (X >= Y) ? X : Y;
    }     

    /**
     * @param Head First comparison value
     * @param Comparators Remaining Comparison values
     * @return Maximum value
     */
    template <typename T, typename... Tail>
    inline constexpr T Max(T Head, Tail... Comparators) noexcept
    {
        return Max(Head, Max(Comparators...));
    }

    /**
     * @param X First comparison value
     * @param Y Second comparison value
     * @return Minimum value of ||X|| and ||Y||
     */
    template <typename T>
    inline constexpr T AbsMin(T X, T Y) noexcept
    {
        auto AbsX = Abs(X);
        auto AbsY = Abs(Y);
        return (AbsX <= AbsY) ? AbsX : AbsY;
    }     

    /**
     * @param Head First comparison value
     * @param Comparators Remaining Comparison values
     * @return Minimum absolute value
     */
    template <typename T, typename... Tail>
    inline constexpr T AbsMin(T Head, Tail... Comparators) noexcept
    {
        return AbsMin(Head, AbsMin(Comparators...));
    } 

    /**
     * @param X First comparison value
     * @param Y Second comparison value
     * @return Maximum value of ||X|| and ||Y||
     */
    template <typename T>
    inline constexpr T AbsMax(T X, T Y) noexcept
    {
        auto AbsX = Abs(X);
        auto AbsY = Abs(Y);
        return (AbsX >= AbsY) ? AbsX : AbsY;
    }     

    /**
     * @param Head First comparison value
     * @param Comparators Remaining Comparison values
     * @return Maximum absolute value
     */
    template <typename T, typename... Tail>
    inline constexpr T AbsMax(T Head, Tail... Comparators) noexcept
    {
        return AbsMax(Head, AbsMax(Comparators...));
    }           

    /**
     *  Calculates the sum of all elements
     * @param Head Head element
     * @param Tails All other elements to add
     * @return Sum of all elements
     */
    template <typename T, typename... Tail>
    inline constexpr auto Sum(T Head, Tail... Tails) noexcept
    {
        return (Head + ... + Tails);
    }

    /** 
     * Calcuates the average of all elements
     * @param Head Leading element
     * @param Tails All other elements
     * @return Average value
     */
    template <typename T, typename... Tail>
    inline constexpr T Average(T Head, Tail... Tails) noexcept
    {
        auto Numerator = Sum(Head, Tails...);
        return Numerator / static_cast<decltype(Numerator)>(sizeof...(Tails) + 1);
    }

    /** 
     * @return Type representation of infinity
     */
    template <typename T>
    inline constexpr T Infinity() noexcept
    {
        return std::numeric_limits<T>::infinity();
    }

    /** 
     * Clamps a value between a certain range. Behaviour is undefined if Lower > Upper
     * @param Val Value to be clamped
     * @param Lower Lower bound
     * @param Upper Upper bound
     * @return `Val` if `Lower` <= `Val` <= `Upper`, else the upper or lower boundary
     */
    template <typename T>
    inline constexpr T Clamp(T Val, T Lower, T Upper) noexcept
    {
        if (Max(Val, Upper) > Upper)
        {
            return Upper;    
        }
        else if (Min(Val, Lower) < Lower)
        {
            return Lower;
        }

        return Val;        
    }
}

using namespace Math;