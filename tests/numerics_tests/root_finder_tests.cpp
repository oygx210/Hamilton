#include "gtest/gtest.h"
#include "tests/test_utils.hpp"
#include "numerics/root1d.hpp"

constexpr double F1(double X)
{
    return Square(X) - 2.0;
}

constexpr double D1(double X)
{
    return 2.0 * X;    
}

constexpr double F2(double X)
{
    return Square(X) + 2.0;
}

// Tries to determine a root using newtons method
TEST(Root, Newton)
{
    // f(x) = x^2 - 2.0    
    {
        constexpr RootFind::RootFinderResult Result = RootFind::Newton(F1, D1, 1.0);

        static_assert(IsNear(Result.X, Sqrt(2.0), 1.0E-8));
        static_assert(Result.ExitCode == RootFind::ExitStatus::SUCCESS);
    }

    // f(x) = x^2 + 2.0
    {
        constexpr RootFind::RootFinderResult Result = RootFind::Newton(F2, D1, 1.0);

        static_assert(Result.ExitCode == RootFind::ExitStatus::MAX_ITERATIONS_EXCEEDED);
    }      
}

// Tries to determine a root using the bisection method
TEST(Root, Bisect)
{
    // f(x) = x^2 - 2.0    
    {
        constexpr RootFind::RootFinderResult Result = RootFind::Bisect(F1, 0.0, 2.0);

        static_assert(IsNear(Result.X, Sqrt(2.0), 1.0E-8));
        static_assert(Result.ExitCode == RootFind::ExitStatus::SUCCESS);
    }

    // f(x) = x^2 + 2.0
    {
        constexpr RootFind::RootFinderResult Result = RootFind::Bisect(F2, 0.0, 2.0);

        static_assert(Result.ExitCode == RootFind::ExitStatus::INVALID_INTERVAL);
    }      
}

// Tries to determine a root using the secant method
TEST(Root, Secant)
{
    // f(x) = x^2 - 2.0    
    {
        constexpr RootFind::RootFinderResult Result = RootFind::Secant(F1, 1.0);

        static_assert(IsNear(Result.X, Sqrt(2.0), 1.0E-8));
        static_assert(Result.ExitCode == RootFind::ExitStatus::SUCCESS);
    }

    // f(x) = x^2 + 2.0
    {
        constexpr RootFind::RootFinderResult Result = RootFind::Secant(F2, 1.0);

        static_assert(Result.ExitCode == RootFind::ExitStatus::MAX_ITERATIONS_EXCEEDED);
    }    
}
