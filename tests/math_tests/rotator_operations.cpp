#include "gtest/gtest.h"
#include "test_utils.hpp"
#include "math/core_math.hpp"
#include "math/rotator.hpp"

TEST(Rotator, BasicOperations)
{
    // For convinience    
    constexpr auto UNIT_X = Vector3::UNIT_X();    
    constexpr auto UNIT_Y = Vector3::UNIT_Y();
    constexpr auto UNIT_Z = Vector3::UNIT_Z();

    // Basic Creation and comparison
    {
        // Default rotator (identity)    
        constexpr Rotator Q1 { };

        // Explicity initialise identity quaternion
        constexpr auto Q2 = Rotator::IDENTITY();                

        // Initialise from components
        constexpr auto Q3 = Rotator({.X = 0.5, .Y = -0.5, .Z = 0.5, .S = 0.5});

        static_assert(Q3.X() == 0.5);
        static_assert(Q3.Y() == -0.5);
        static_assert(Q3.Z() == 0.5);
        static_assert(Q3.S() == 0.5);

        // Not equal
        static_assert(Q3 != Q2);

        // copy
        constexpr auto Q4 = Q1;
        static_assert(Q1 == Q4);
    }

    // Operations on quaternions
    {
        constexpr auto Q1 = Rotator({.X=1.0, .Y = 0.0, .Z = -1.0, .S = 1.0});

        constexpr auto Q2 = Rotator({.X=-1.0, .Y=0.0, .Z=1.0, .S=1.0});
        static_assert(Q1.Inverse() == Q2);
    }    

    // More complex construction, rotation    
    {
        // rotation PI / 2 from x -> y axis    
        constexpr auto Q1 = Rotator::FromVectorPair(UNIT_X,  // frame O
                                                    UNIT_Y); // frame M


        // x axis in frame O is y axis in frame M by definition
        static_assert(IsVector3Near(Q1.Rotate(UNIT_X), UNIT_Y, 1.0E-15));
        static_assert(IsVector3Near(Q1.Rotate(-UNIT_X), -UNIT_Y, 1.0E-15));
        static_assert(IsVector3Near(Q1.RotateInv(UNIT_Y), UNIT_X, 1.0E-15));
        static_assert(IsVector3Near(Q1.RotateInv(-UNIT_Y), -UNIT_X, 1.0E-15));

        // y axis in frame O should be -x axis in frame M
        static_assert(IsVector3Near(Q1.Rotate(UNIT_Y), -UNIT_X, 1.0E-15));
        static_assert(IsVector3Near(Q1.RotateInv(-UNIT_X), UNIT_Y, 1.0E-15));

        // +- z axis should remain the same
        static_assert(IsVector3Near(Q1.Rotate(UNIT_Z),  UNIT_Z, 1.0E-15));
        static_assert(IsVector3Near(Q1.Rotate(-UNIT_Z), -UNIT_Z, 1.0E-15));
        static_assert(IsVector3Near(Q1.RotateInv(UNIT_Z), UNIT_Z, 1.0E-15));
        static_assert(IsVector3Near(Q1.RotateInv(-UNIT_Z), -UNIT_Z, 1.0E-15));

        // test a more complicated vector
        static_assert(IsVector3Near(Q1.Rotate(Vector3({1, 1, 1})), Vector3({-1.0, 1.0, 1.0}), 1.0E-15));

        // Same vector now defined from vector angle initialisation
        constexpr auto Q2 = Rotator::FromVectorAngle(UNIT_Z, -0.5 * PI);

        static_assert(IsQuaternionNear(Q1.AsQuaternion(), Q2.AsQuaternion(), 1.0E-15));
    }

    {
        // Another more complicated case
        constexpr auto U = Vector3({1.0, 1.0, 1.0});
        constexpr auto Q = Rotator({.X = -0.59037304186825678709, 
                          .Y = 0.737381763784046517785, 
                          .Z = 0.0440496593533170818779, 
                          .S = 0.32524980151386218008});
        constexpr auto Q2 = Q.Inverse();

        constexpr auto V2 = Vector3({-1.46502882737748851838, -0.919350916194919598468, 0.0921109539875909488771});
        constexpr auto V3 = Q.Rotate(U);

        static_assert(IsVector3Near(V3, V2, 1.0E-15));

        // Should recover original vector from inverse transformation
        static_assert(IsVector3Near(Q.RotateInv(V3), U, 1.0E-15));
        static_assert(IsVector3Near(Q2.Rotate(V3), U, 1.0E-15));
    }

    {
        // Check the quaternion itself
        constexpr auto U = Vector3({-1.0, -1.0, -1.0});
        constexpr auto V = Vector3({3.0, -2.0, 1.0});
        constexpr auto Q = Rotator({
            .X = -0.3936579516353742, 
            .Y = -0.26243863442358284, 
            .Z = 0.65609658605895704, 
            .S = 0.58795973504816468});

        static_assert(IsQuaternionNear(Q.AsQuaternion(), Quaternion::FromVectorPair(V, U), 1.0E-12));
    }

    {
        // Test sucessive rotations

        // Rotate z axis counter clockwise, x -> -y, y -> x
        constexpr auto Q = Quaternion::FromVectorAngle(UNIT_Z, 0.5 * PI);

        // Rotate y axis counter clockwise, z-> -x, x-> z
        constexpr auto Q2 = Quaternion::FromVectorAngle(UNIT_Y, 0.5 * PI);

        // Net transform x->-y->-y, y->x->z, z->z->-x
        constexpr auto Q3 = Rotator(Q * Q2);

        static_assert(IsVector3Near(Q3.UnitX(), -UNIT_Y, 1.0E-15));
        static_assert(IsVector3Near(Q3.UnitY(), UNIT_Z, 1.0E-15));
        static_assert(IsVector3Near(Q3.UnitZ(), -UNIT_X, 1.0E-15));
    }
    
    // Euler angles
    {
        // Identity    
        {
            constexpr auto Q = Rotator::IDENTITY();
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }

        // X axis        

        // Rotation PI/2 rad about x    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_X(), PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation -PI/2 rad about x    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_X(), -PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, -PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation 3*PI/2 rad about x    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_X(), 1.5 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, -0.5 * PI, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }          

        // Rotation 2*PI rad about x    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_X(), 2.0 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, 0.0, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }   

        // Y axis

        // Rotation PI/2 rad about y    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Y(), PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation -PI/2 rad about y    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Y(), -PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, -PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation 3*PI/2 rad about y
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Y(), 1.5 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, -0.5 * PI, 1.0E-15));
            static_assert(IsNear(Fmod(EulerAngles.Z, (2.0 * PI)), 0.0, 1.0E-15)); // fp error puts this one at ~=-2 pi
        }          

        // Rotation 2*PI rad about y  
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Y(), 2.0 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, 0.0, 1.0E-15));
            static_assert(EulerAngles.Z == 0.0);
        }       

        // Z axis

        // Rotation PI/2 rad about z
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Z(), PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, 0.5 * PI, 1.0E-15));
        }        

        // Rotation -PI/2 rad about z    
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Z(), -PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, -0.5 * PI, 1.0E-15));
        }        

        // Rotation 3*PI/2 rad about z
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Z(), 1.5 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, -0.5 * PI, 1.0E-15));
        }          

        // Rotation 2*PI rad about z  
        {
            constexpr auto Q = Rotator::FromVectorAngle(Vector3::UNIT_Z(), 2.0 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, 0.0, 1.0E-15));
        }         

        // Some composed rotations
        {
            constexpr auto Q1 = Rotator::FromVectorAngle(Vector3::UNIT_Z(), 0.25 * PI);
            constexpr auto Q2 = Rotator::FromVectorAngle(Vector3::UNIT_Y(), PI / 6.0);
            constexpr auto Q3 = Rotator::FromVectorAngle(Vector3::UNIT_X(), PI);
            constexpr auto Q = Rotator::Compose(Q1, Q2, Q3);

            constexpr auto EulerAngles = Q.EulerAngles();

            static_assert(IsNear(EulerAngles.X, PI, 1.0E-15));
            static_assert(IsNear(EulerAngles.Y, PI / 6.0, 1.0E-15));
            static_assert(IsNear(EulerAngles.Z, 0.25 * PI, 1.0E-15));
        }                                        
    }    

    // Direct Cosine Matrix (TODO)
    {
        // A simple orthogonal transform    
        {    
            // Rotation : x -> x, y -> z, z -> -y
            constexpr auto Q1 = Rotator::FromVectorAngle(Vector3::UNIT_X(), 0.5 * PI);
                
            constexpr auto Rot = Q1.DirectCosineMatrix();

            constexpr auto UnitX = Vector3({.X = Rot.XX, .Y = Rot.YX, .Z = Rot.ZX});
            constexpr auto UnitY = Vector3({.X = Rot.XY, .Y = Rot.YY, .Z = Rot.ZY});
            constexpr auto UnitZ = Vector3({.X = Rot.XZ, .Y = Rot.YZ, .Z = Rot.ZZ});

            static_assert(IsVector3Near(UnitX, Q1.UnitX(), 1.0E-15));
            static_assert(IsVector3Near(UnitY, Q1.UnitY(), 1.0E-15));
            static_assert(IsVector3Near(UnitZ, Q1.UnitZ(), 1.0E-15));
        }

        // More complex transform
        {
            constexpr auto Q1 = Rotator({.X = 0.507, .Y = 0.507, .Z = 0.507, .S = 0.159});
            constexpr auto Rot = Q1.DirectCosineMatrix();            

            constexpr auto UnitX = Vector3({.X = Rot.XX, .Y = Rot.YX, .Z = Rot.ZX});
            constexpr auto UnitY = Vector3({.X = Rot.XY, .Y = Rot.YY, .Z = Rot.ZY});
            constexpr auto UnitZ = Vector3({.X = Rot.XZ, .Y = Rot.YZ, .Z = Rot.ZZ});

            static_assert(IsVector3Near(UnitX, Q1.UnitX(), 1.0E-15));
            static_assert(IsVector3Near(UnitY, Q1.UnitY(), 1.0E-15));
            static_assert(IsVector3Near(UnitZ, Q1.UnitZ(), 1.0E-15));
        }        
    }   
}