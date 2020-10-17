#pragma once

/** 
 * @file root1d.hpp
 */

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
        RootFinderResult Result{.X = Guess};

        for (int Index = 0; Index < Parameters.MaxIterations; Index++)
        {
            Result.Delta = Parameters.Relaxation * Function(Result.X) / Derivative(Result.X);

            // Converged
            if (Abs(Result.Delta) < Parameters.Tolerance)
            {
                Result.ExitCode = ExitStatus::SUCCESS;
                Result.Iterations = Index;
                return Result;
            }

            Result.X -= Result.Delta;
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
}
