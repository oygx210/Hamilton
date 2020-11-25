#include "gtest/gtest.h"
#include "test_utils.hpp"
#include "math/core_math.hpp"
#include "math/matrix3.hpp"

TEST(Matrix3, BasicOperations)
{
    // Default matrix constructors    
    {
        static_assert(Matrix3::IDENTITY().XX == 1.0);
        static_assert(Matrix3::IDENTITY().XY == 0.0);
        static_assert(Matrix3::IDENTITY().XZ == 0.0);
        static_assert(Matrix3::IDENTITY().YX == 0.0);
        static_assert(Matrix3::IDENTITY().YY == 1.0);
        static_assert(Matrix3::IDENTITY().YZ == 0.0);
        static_assert(Matrix3::IDENTITY().ZX == 0.0);
        static_assert(Matrix3::IDENTITY().ZY == 0.0);
        static_assert(Matrix3::IDENTITY().ZZ == 1.0);

        static_assert(Matrix3::ZERO().XX == 0.0);
        static_assert(Matrix3::ZERO().XY == 0.0);
        static_assert(Matrix3::ZERO().XZ == 0.0);
        static_assert(Matrix3::ZERO().YX == 0.0);
        static_assert(Matrix3::ZERO().YY == 0.0);
        static_assert(Matrix3::ZERO().YZ == 0.0);
        static_assert(Matrix3::ZERO().ZX == 0.0);
        static_assert(Matrix3::ZERO().ZY == 0.0);
        static_assert(Matrix3::ZERO().ZZ == 0.0);

        static_assert(Matrix3{}.XX == 0.0);
        static_assert(Matrix3{}.XY == 0.0);
        static_assert(Matrix3{}.XZ == 0.0);
        static_assert(Matrix3{}.YX == 0.0);
        static_assert(Matrix3{}.YY == 0.0);
        static_assert(Matrix3{}.YZ == 0.0);
        static_assert(Matrix3{}.ZX == 0.0);
        static_assert(Matrix3{}.ZY == 0.0);
        static_assert(Matrix3{}.ZZ == 0.0);
    }    

    // Matrix equality
    {
        constexpr auto Mat1 = Matrix3::IDENTITY();
        auto Mat2 = Matrix3::IDENTITY();

        ASSERT_TRUE(Mat1 == Mat2);
        ASSERT_FALSE(Mat1 != Mat2);

        Mat2.XY = 2.0;

        ASSERT_FALSE(Mat1 == Mat2);
        ASSERT_TRUE(Mat1 != Mat2);
    }

    // Matrix Scalar Multiplication    
    {
        constexpr auto Mat = Matrix3 {
            .XX = 2.0,  .XY = 3.0, .XZ = -4.0,
            .YX = 11.0, .YY = 8.0, .YZ = 7.0,
            .ZX = 2.0,  .ZY = 5.0, .ZZ = 3.0};

        constexpr auto Mat2 = Matrix3 {
            .XX = 4.0,  .XY = 6.0, .XZ = -8.0,
            .YX = 22.0, .YY = 16.0, .YZ = 14.0,
            .ZX = 4.0,  .ZY = 10.0, .ZZ = 6.0};                            

        constexpr auto Mat3 = 2.0 * Mat;
        constexpr auto Mat4 = Mat * 2.0;

        static_assert(Mat2 == Mat3);
        static_assert(Mat2 == Mat4);
    }

    // Matrix Vector Multiplication
    {
        
        // Zero matrix mult
        {
            constexpr auto Mat = Matrix3::ZERO();
            constexpr auto Vct = Vector3({40.0, 1257.353, -1287.0});
            constexpr auto Prd = Mat * Vct;
            static_assert(Prd == Vector3::ZERO());
        }        

        // Zero vector mult    
        {
            constexpr auto Mat = Matrix3 {.XX = 2.0,  .XY = 3.0, .XZ = -4.0,
                                .YX = 11.0, .YY = 8.0, .YZ = 7.0,
                                .ZX = 2.0,  .ZY = 5.0, .ZZ = 3.0};
            constexpr auto Vct = Vector3::ZERO();
            constexpr auto Prd = Mat * Vct;

            static_assert(Prd == Vector3::ZERO());
        }    

        // diagonal    
        {
            constexpr auto Mat = Matrix3 {.XX = 1.0, .YY = 2.0, .ZZ = 4.0};
            constexpr auto Vct = Vector3({-2.0, 4.0, -3.0});

            constexpr auto Prd = Mat * Vct;

            static_assert(IsVector3Near(Prd, Vector3({-2.0, 8.0, -12.0}), 1.0E-15));
        }   

        // dense matrix
        {    
            constexpr auto Mat = Matrix3 {.XX = 2.0,  .XY = 3.0, .XZ = -4.0,
                                .YX = 11.0, .YY = 8.0, .YZ = 7.0,
                                .ZX = 2.0,  .ZY = 5.0, .ZZ = 3.0};
            constexpr auto Vct = Vector3 ({3.0, 7.0, 5.0});

            constexpr auto Prd = Mat * Vct;

            static_assert(IsVector3Near(Prd, Vector3({7.0, 124.0, 56.0}), 1.0E-15));
        }

    }

    // Matrix-Matrix Multiplication
    {
        {    
            constexpr auto Mat1 = Matrix3{
                .XX = 4.0,  .XY = 11.0, .XZ = -27.0,
                .YX = 7.0,  .YY = 0.0,  .YZ = 5.0,
                .ZX = -6.0, .ZY = 8.0,  .ZZ = -67.0};

            constexpr auto Mat2 = Matrix3{
                .XX = 3.0,  .XY = 0.0,  .XZ = -8.0,
                .YX = 51.0, .YY = -7.0, .YZ = 54.0,
                .ZX = 3.0,  .ZY = 0.0,  .ZZ = 2.0};

            constexpr auto Mat3 = Matrix3{
                .XX = 492.0, .XY = -77.0, .XZ = 508.0,
                .YX = 36.0,  .YY = 0.0,   .YZ = -46.0,
                .ZX = 189.0, .ZY = -56.0, .ZZ = 346.0};

            constexpr auto Mat4 = Mat1 * Mat2;                                

            static_assert(IsMatrix3Near(Mat3, Mat4, 1.0E-15));
        }
    }

    // Transpose
    {
        {
            constexpr auto Mat1 = Matrix3{
                .XX = 4.0,  .XY = 11.0, .XZ = -27.0,
                .YX = 7.0,  .YY = 0.0,  .YZ = 5.0,
                .ZX = -6.0, .ZY = 8.0,  .ZZ = -67.0};

            constexpr auto Mat2 = Matrix3{
                .XX = 4.0,  .XY = 7.0, .XZ = -6.0,
                .YX = 11.0,  .YY = 0.0,  .YZ = 8.0,
                .ZX = -27.0, .ZY = 5.0,  .ZZ = -67.0};                                

            static_assert(Mat2 == Mat1.Transpose());
        }
    }

    // Matrix addition, subtraction, negation
    {
        {
            constexpr auto Mat1 = Matrix3 {.XX = 2.0,  .XY = 3.0, .XZ = -4.0,
                                .YX = 11.0, .YY = 8.0, .YZ = 7.0,
                                .ZX = 2.0,  .ZY = 5.0, .ZZ = 3.0};

            constexpr auto Mat2 = -Mat1;
            constexpr auto Mat3 = -1.0 * Mat1;

            static_assert(Mat2 == Mat3);

            constexpr auto Mat4 = Mat1 + Mat2;
            static_assert(Mat4 == Matrix3::ZERO());

            constexpr auto Mat5 = Mat3 - Mat2;
            static_assert(Mat5 == Matrix3::ZERO());
        }
    }
}
