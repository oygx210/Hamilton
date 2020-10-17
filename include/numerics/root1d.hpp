#pragma once

/** 
 * @file root1d.hpp
 */

#include <cstdio>
#include "math/core_math.hpp"

namespace RootFind
{
    /** 
     * A collection of exit flags for a root search algorithm
     */
    enum struct ExitStatus
    {
        /// Miscellaneous unclassified error    
        OTHER_ERROR,

        /// Solver converged    
        SUCCESS,

        /// Solver did not coverge in the maximum number of iterations allowed
        MAX_ITERATIONS_EXCEEDED,

        /// An unstable or unsolvable problem was detected    
        ILL_POSED,

        /// Invalid inputs were detected for one or more of the provided parameters    
        INVALID_PARAMETERS,

        /// The provided interval is invalid
        INVALID_INTERVAL    
    };

    /** 
     * Root search exit struct
     */
    struct RootFinderResult
    {
        double X = 0.0;
        double Delta = 0.0;
        int Iterations = 0;
        ExitStatus ExitCode = ExitStatus::OTHER_ERROR;
    };

    /** 
     * Newton solver input parameters
     */
    struct NewtonParameters
    { 
        /// Procedure will exit successfully once fabs(delta) < `Tolerance`
        double Tolerance = 1.0E-8;

        /// Procedure will exit with error if this many iterations exceeded
        int MaxIterations = 16;

        /// Convergence assistance factor    
        double Relaxation = 1.0;
    };

    /** 
     * Bisection solver input parameters
     */
    struct BoundedParameters
    {
        /// Procedure will exit successfully once fabs(delta) < `Tolerance`
        double Tolerance = 1.0E-8;

        /// Procedure will exit with error if this many iterations exceeded
        int MaxIterations = 128;
    };

    /// Default Newtonian solver inputs
    constexpr auto DefaultNewtonParameters = NewtonParameters{};

    /// Default Bisection solver inputs
    constexpr auto DefaultBoundedParameters = BoundedParameters{};    

    /** 
     * Attempts to determine the root x of a zero function f(x) = 0 
     * using Newtonian iteration. 
     * NOTE: Will not check for f'(x) = 0, which will cause a convergence failure. If this is possible, mitigations 
     * should be built into the input function 
     * @param Function Function f(x) to determine the root of 
     * @param Derivative Function derivative f'(x) to determine the root of
     * @param Guess Initial guess for the root
     * @param Parameters Additional solver parameter
     * @return RootFinderResult
     */
    constexpr RootFinderResult Newton(
        const auto Function, 
        const auto Derivative, 
        double Guess,
        const NewtonParameters& Parameters = DefaultNewtonParameters) noexcept
    {
        RootFinderResult Result{.X = Guess, .Delta = Function(Guess) / Derivative(Guess)};

        for (int Index = 0; Index < Parameters.MaxIterations; Index++)
        {
            Result.X -= Result.Delta;

            // Converged
            if (Abs(Result.Delta) < Parameters.Tolerance)
            {
                Result.ExitCode = ExitStatus::SUCCESS;
                Result.Iterations = Index;
                return Result;
            }

            Result.Delta = Parameters.Relaxation * Function(Result.X) / Derivative(Result.X);
        }

        // Did not converge
        Result.X -= Result.Delta;
        Result.Iterations = Parameters.MaxIterations;

        // Invalid inputs
        if (
            (Parameters.Tolerance < 0.0) ||
            (Parameters.MaxIterations < 1) ||
            (Parameters.Relaxation < 0.0)
        )
        {
            Result.ExitCode = ExitStatus::INVALID_PARAMETERS;
        }
        // Max Iterations exceeded
        else
        {
            Result.ExitCode = ExitStatus::MAX_ITERATIONS_EXCEEDED;
        }

        return Result;
    }

    /** 
     * Attempts to determine the root x of a zero function f(x) = 0 
     * using the bisection method
     * @param Function Function f(x) to determine the root of 
     * @param X1 Lower bound for the interval
     * @param X2 Upper bound for the interval
     * @param Parameters Additional solver parameter
     * @return RootFinderResult
     */
    constexpr RootFinderResult Bisect(
        const auto Function,
        double X1,
        double X2,
        const BoundedParameters& Parameters = DefaultBoundedParameters
    )
    {
        double F1 = Function(X1);
        double F2 = Function(X2);

        RootFinderResult Result{.X = 0.5 * (X1 + X2), .Delta = X2 - X1};        

        if (F1 == 0)
        {
            return RootFinderResult{.X = X1, .Delta = X2 - X1, .ExitCode = ExitStatus::SUCCESS};
        }
        else if (F2 == 0)
        {
            return RootFinderResult{.X = X2, .Delta = X2 - X1, .ExitCode = ExitStatus::SUCCESS};
        }
        else if (F1 * F2 > 0.0)
        {
            return RootFinderResult{.X = Result.X, .Delta = X2 - X1, .ExitCode = ExitStatus::INVALID_INTERVAL};    
        }
        else if (X2 - X1 <= 0.0)
        {
            Result.ExitCode = ExitStatus::INVALID_INTERVAL;
            return Result;
        }

        double F3 = Function(Result.X);

        for (int Index = 0; Index < Parameters.MaxIterations; Index++)
        {
            if (F3 == 0.0)
            {
                Result.Delta = (X2 - X1) * 0.5;
                Result.ExitCode = ExitStatus::SUCCESS;
                Result.Iterations = Index;
                return Result;
            }
            else if (F1 * F3 < 0.0)
            {
                X2 = Result.X;
                F2 = F3;
            }
            else
            {
                X1 = Result.X;
                F1 = F3;
            }

            Result.X = 0.5 * (X1 + X2);
            Result.Delta = X2 - X1;

            if (Abs(Result.Delta) < Parameters.Tolerance)
            {
                Result.ExitCode = ExitStatus::SUCCESS;
                Result.Iterations = Index;
                return Result; 
            }

            F3 = Function(Result.X);
        }

        Result.Iterations = Parameters.MaxIterations;        

        // Invalid inputs
        if (
            (Parameters.Tolerance < 0.0) ||
            (Parameters.MaxIterations < 1)
        )
        {
            Result.ExitCode = ExitStatus::INVALID_PARAMETERS;
        }
        // Max Iterations exceeded
        else
        {
            Result.ExitCode = ExitStatus::MAX_ITERATIONS_EXCEEDED;
        }

        return Result;     
    }

    /** 
     * Attempts to determine the root x of a zero function f(x) = 0 
     * using the secant method. 
     * @param Function Function f(x) to determine the root of 
     * @param Guess Initial guess for the root
     * @param Parameters Additional solver parameter
     * @return RootFinderResult
     */
    constexpr RootFinderResult Secant(
        const auto Function, 
        double Guess,
        const NewtonParameters& Parameters = DefaultNewtonParameters) noexcept
    {
        double Xp = Guess;
        RootFinderResult Result {.X = (Xp >= 0) ? Xp * (1.0 + 1.0E-4) + 1.0E-4 : Xp * (1.0 + 1.0E-4) - 1.0E-4};
        double Yp = Function(Xp);
        double Yn = Function(Result.X);
        Result.Delta = (Result.X - Xp) / (Yn - Yp) * Yn * Parameters.Relaxation;

        for (int Index = 0; Index < Parameters.MaxIterations; Index++)
        {
            // Success    
            if (Abs(Result.Delta) < Parameters.Tolerance)
            {
                Result.Iterations = Index;
                Result.ExitCode = ExitStatus::SUCCESS;
                Result.X -= Result.Delta;
                return Result;
            }

            // Ill posed breakout
            if (Yn - Yp == 0.0)
            {
                Result.Iterations = Index;
                Result.ExitCode = ExitStatus::ILL_POSED;
                Result.X -= Result.Delta;
                return Result;    
            }

            Xp = Result.X;
            Result.X -= Result.Delta;
            Yp = Yn;
            Yn = Function(Result.X);
            Result.Delta = (Result.X - Xp) / (Yn - Yp) * Yn * Parameters.Relaxation;
        }

        // Did not converge
        Result.Iterations = Parameters.MaxIterations;

        // Invalid inputs
        if (
            (Parameters.Tolerance < 0.0) ||
            (Parameters.MaxIterations < 1) ||
            (Parameters.Relaxation < 0.0)
        )
        {
            Result.ExitCode = ExitStatus::INVALID_PARAMETERS;
        }
        // Max Iterations exceeded
        else
        {
            Result.ExitCode = ExitStatus::MAX_ITERATIONS_EXCEEDED;
        }

        return Result;        
    }

    /** 
     * Attempts to determine the root x of a zero function f(x) = 0 
     * using ridders method.
     * (Ridders, C. (1979). "A new algorithm for computing a single 
     * root of a real continuous function". IEEE Transactions on Circuits and Systems. 
     * 26: 979–980). Simple bracketing method with an order of convergence ~sqrt(2). 
     * @param Function Function f(x) to determine the root of 
     * @param X1 Lower bound for the interval
     * @param X2 Upper bound for the interval
     * @param Parameters Additional solver parameter
     * @return RootFinderResult
     */
    // constexpr RootFinderResult Ridder(
    //     const auto Function,
    //     double X1,
    //     double X2,
    //     const BoundedParameters& Parameters = DefaultBoundedParameters
    // )
    // {
    //     double F1 = Function(X1);
    //     double F2 = Function(X2);

    //     if (F1 * F2 > 0)
    //     {
    //         return RootFinderResult{.X = 0.5 * (X1 + X2), .Delta = X2 - X1, .ExitCode = ExitStatus::INVALID_INTERVAL};
    //     }   

    //     RootFinderResult Result{.X = 0.5 * (X1 + X2)};

    //     for (int Index = 0; Index < Parameters.MaxIterations; Index++)
    //     {
    //         double F3 = Function(Result.X);

    //         if (F3 == 0.0)
    //         {
    //             Result.Iterations = Index;    
    //             Result.ExitCode = ExitStatus::SUCCESS;    
    //         }

    //         double DenomSquared = Square(F3) - F1 * F2;
    //         // double Eps = (F3 + Signum(F2) * Sqrt(DenomSquared)) / F2;

    //         // double X4 = Result.X + (Result.X - X1) * F3 * Eps / (F1 - Eps * F3);
    //         double X4 = Result.X + (Result.X - X1) * Signum(X1) * F3 / Sqrt(DenomSquared);
    //         Result.Delta = AbsMin(X4 - X1, X4 - X2);

    //         // Convergence
    //         if (Result.Delta < Parameters.Tolerance)
    //         {
    //             Result.Iterations = Index;
    //             Result.ExitCode = ExitStatus::SUCCESS;    
    //             return Result;
    //         }    

    //         double F4 = Function(X4);

    //         // Bracket is the root
    //         if (F4 == 0.0)
    //         {
    //             Result.X = X4;
    //             Result.Iterations = Index;
    //             Result.ExitCode = ExitStatus::SUCCESS;
    //             return Result;    
    //         }

    //         // Select next bracket
    //         if ( F3 * F2 < 0.0 ) 
    //         {
    //             if (F4 * F2 < 0.0 ) 
    //             {
    //                 X1 = X4; 
    //                 F1 = F4;
    //             } 
    //             else 
    //             {
    //                 X1 = Result.X; 
    //                 F1 = F3;
    //                 X2 = X4; 
    //                 F2 = F4;
    //             }
    //         } 
    //         else 
    //         {
    //             if ( F4 * F1 < 0.0 )
    //             {
    //                 X2 = X4; 
    //                 F2 = F4;
    //             }
    //             else 
    //             {
    //                 X1 = X4; 
    //                 F1 = F4;
    //                 X2 = Result.X;
    //                 F2 = F3;
    //             }
    //         }            

    //         Result.X = 0.5 * (X1 + X2);                      
    //     }

    //     Result.Iterations = Parameters.MaxIterations;        

    //     // Invalid inputs
    //     if (
    //         (Parameters.Tolerance < 0.0) ||
    //         (Parameters.MaxIterations < 1)
    //     )
    //     {
    //         Result.ExitCode = ExitStatus::INVALID_PARAMETERS;
    //     }
    //     // Max Iterations exceeded
    //     else
    //     {
    //         Result.ExitCode = ExitStatus::MAX_ITERATIONS_EXCEEDED;
    //     }

    //     return Result;             
    // }            
}