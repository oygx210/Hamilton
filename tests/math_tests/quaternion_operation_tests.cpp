#include "gtest/gtest.h"
#include "test_utils.hpp"
#include "math/core_math.hpp"
#include "math/quaternion.hpp"

TEST(Quaternion, BasicOperations)
{
    // For convinience    
    constexpr auto UNIT_X = Vector3::UNIT_X();    
    constexpr auto UNIT_Y = Vector3::UNIT_Y();
    constexpr auto UNIT_Z = Vector3::UNIT_Z();

    {   
        // Extra checks to guard against accidental overrides
        static_assert(Quaternion{} == Quaternion::IDENTITY());

        static_assert(Quaternion::IDENTITY().X == 0.0);
        static_assert(Quaternion::IDENTITY().Y == 0.0);
        static_assert(Quaternion::IDENTITY().Z == 0.0);
        static_assert(Quaternion::IDENTITY().S == 1.0);

        static_assert(Quaternion::ZERO().X == 0.0);
        static_assert(Quaternion::ZERO().Y == 0.0);
        static_assert(Quaternion::ZERO().Z == 0.0);
        static_assert(Quaternion::ZERO().S == 0.0);
    }    

    // Basic Creation and comparison
    {
        // Default quaternion (identity)    
        constexpr auto Q1 = Quaternion {};

        // Explicity initialise identity quaternion
        constexpr auto Q2 = Quaternion::IDENTITY();                

        // Initialise from components
        constexpr auto Q3 = Quaternion {.X=0.5, .Y=-0.5, .Z=0.5, .S=1.0};

        static_assert(Q3.X == 0.5);
        static_assert(Q3.Y == -0.5);
        static_assert(Q3.Z == 0.5);
        static_assert(Q3.S == 1.0);

        // Not equal
        static_assert(Q3 != Q2);

        // copy
        constexpr auto Q4 = Q1;
        static_assert(Q1 == Q4);

        // Move
        constexpr auto Q5 = Quaternion {.X = 1.0, .Y=0.0, .Z=0.0, .S=0.0};
        static_assert(Q5.X == 1.0);
        static_assert(Q5.Y == 0.0);
        static_assert(Q5.Z == 0.0);
        static_assert(Q5.S == 0.0);
    }

    // Operations on quaternions
    {
        constexpr auto Q1 = Quaternion{.X=1.0, .Y = 0.0, .Z = -1.0, .S = 1.0};

        static_assert(Q1.NormSquared() == 3.0);
        static_assert(Q1.Norm() == Sqrt(3.0));

        constexpr auto Q2 = Quaternion{.X=-1.0, .Y=0.0, .Z=1.0, .S=1.0};
        static_assert(Q1.Inverse() == Q2.Unit());
    }    

    // More complex construction, rotation    
    {
        // rotation PI / 2 from x -> y axis    
        constexpr auto Q1 = Quaternion::FromVectorPair(UNIT_X,  // frame O
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
        constexpr auto Q2 = Quaternion::FromVectorAngle(UNIT_Z, -0.5 * PI);

        static_assert(IsQuaternionNear(Q1, Q2, 1.0E-15));

        // Both fromVectorAngle and fromVectorPair should always constuct a unit quaternion
        constexpr auto Q3 = Quaternion::FromVectorPair(Vector3({24, 35, -101}), Vector3({54, -193, 982}));
        static_assert(IsNear(Q3.NormSquared(), 1.0, 1.0E-15));

        constexpr auto Q4 = Quaternion::FromVectorAngle(Vector3({268, -172, 345}), -2677280.0);
        static_assert(IsNear(Q4.NormSquared(), 1.0, 1.0E-15));
    }

    {
        // Another more complicated case
        constexpr auto U = Vector3({1.0, 1.0, 1.0});
        constexpr auto Q = Quaternion{.X = -0.59037304186825678709, 
                            .Y = 0.737381763784046517785, 
                            .Z = 0.0440496593533170818779, 
                            .S = 0.32524980151386218008};
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
        constexpr auto Q = Quaternion{
            .X = -0.3936579516353742, 
            .Y = -0.26243863442358284, 
            .Z = 0.65609658605895704, 
            .S = 0.58795973504816468};

        static_assert(IsQuaternionNear(Q, Quaternion::FromVectorPair(V, U), 1.0E-12));
    }

    {
        // Test sucessive rotations

        // Rotate z axis counter clockwise, x -> -y, y -> x
        constexpr auto Q = Quaternion::FromVectorAngle(UNIT_Z, 0.5 * PI);

        static_assert(IsVector3Near(Q.Rotate(UNIT_X), -UNIT_Y, 1.0E-15));
        static_assert(IsVector3Near(Q.Rotate(UNIT_Y), UNIT_X, 1.0E-15));
        static_assert(IsVector3Near(Q.Rotate(UNIT_Z), UNIT_Z, 1.0E-15));

        // Roate y axis counter clockwise, z-> -x, x-> z
        constexpr auto Q2 = Quaternion::FromVectorAngle(UNIT_Y, 0.5 * PI);

        // Net transform x->-y->-y, y->x->z, z->z->-x
        constexpr auto Q3 = Q * Q2;

        static_assert(IsVector3Near(Q3.Rotate(UNIT_X), -UNIT_Y, 1.0E-15));
        static_assert(IsVector3Near(Q3.Rotate(UNIT_Y), UNIT_Z, 1.0E-15));
        static_assert(IsVector3Near(Q3.Rotate(UNIT_Z), -UNIT_X, 1.0E-15));
    }
    
    // Euler angles
    {
        // Identity    
        {
            constexpr auto Q = Quaternion::IDENTITY();
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }

        // X axis        

        // Rotation PI/2 rad about x    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_X(), PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation -PI/2 rad about x    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_X(), -PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, -PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation 3*PI/2 rad about x    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_X(), 1.5 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, -0.5 * PI, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }          

        // Rotation 2*PI rad about x    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_X(), 2.0 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(IsNear(EulerAngles.X, 0.0, 1.0E-15));
            static_assert(EulerAngles.Y == 0.0);
            static_assert(EulerAngles.Z == 0.0);
        }   

        // Y axis

        // Rotation PI/2 rad about y    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Y(), PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation -PI/2 rad about y    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Y(), -PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, -PI * 0.5, 1.0E-15));
            static_assert(EulerAngles.Z == 0.0);
        }        

        // Rotation 3*PI/2 rad about y
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Y(), 1.5 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, -0.5 * PI, 1.0E-15));
            static_assert(IsNear(Fmod(EulerAngles.Z, (2.0 * PI)), 0.0, 1.0E-15)); // fp error puts this one at ~=-2 pi
        }          

        // Rotation 2*PI rad about y  
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Y(), 2.0 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(IsNear(EulerAngles.Y, 0.0, 1.0E-15));
            static_assert(EulerAngles.Z == 0.0);
        }       

        // Z axis

        // Rotation PI/2 rad about z
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, 0.5 * PI, 1.0E-15));
        }        

        // Rotation -PI/2 rad about z    
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), -PI * 0.5);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, -0.5 * PI, 1.0E-15));
        }        

        // Rotation 3*PI/2 rad about z
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), 1.5 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, -0.5 * PI, 1.0E-15));
        }          

        // Rotation 2*PI rad about z  
        {
            constexpr auto Q = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), 2.0 * PI);
            constexpr auto EulerAngles = Q.EulerAngles();
            static_assert(EulerAngles.X == 0.0);
            static_assert(EulerAngles.Y == 0.0);
            static_assert(IsNear(EulerAngles.Z, 0.0, 1.0E-15));
        }         

        // Some composed rotations
        {
            constexpr auto Q1 = Quaternion::FromVectorAngle(Vector3::UNIT_Z(), 0.25 * PI);
            constexpr auto Q2 = Quaternion::FromVectorAngle(Vector3::UNIT_Y(), PI / 6.0);
            constexpr auto Q3 = Quaternion::FromVectorAngle(Vector3::UNIT_X(), PI);
            constexpr auto Q = Q1 * Q2 * Q3;

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
            constexpr auto Q1 = Quaternion::FromVectorAngle(Vector3::UNIT_X(), 0.5 * PI);
                
            constexpr auto Rot = Q1.DirectCosineMatrix();

            constexpr auto UnitX = Vector3({.X = Rot.XX, .Y = Rot.YX, .Z = Rot.ZX});
            constexpr auto UnitY = Vector3({.X = Rot.XY, .Y = Rot.YY, .Z = Rot.ZY});
            constexpr auto UnitZ = Vector3({.X = Rot.XZ, .Y = Rot.YZ, .Z = Rot.ZZ});

            constexpr auto ExpectedUnitX = Vector3::UNIT_X(); // Q1.RotateInv(Vector3::UNIT_X());
            constexpr auto ExpectedUnitY = Q1.Rotate(Vector3::UNIT_Y());
            constexpr auto ExpectedUnitZ = Q1.Rotate(Vector3::UNIT_Z());

            static_assert(IsVector3Near(UnitX, ExpectedUnitX, 1.0E-15));
            static_assert(IsVector3Near(UnitY, ExpectedUnitY, 1.0E-15));
            static_assert(IsVector3Near(UnitZ, ExpectedUnitZ, 1.0E-15));
        }

        // More complex transform
        {
            constexpr auto Q1 = Quaternion{.X = 0.507, .Y = 0.507, .Z = 0.507, .S = 0.159}.Unit();
            constexpr auto Rot = Q1.DirectCosineMatrix();            

            constexpr auto UnitX = Vector3({.X = Rot.XX, .Y = Rot.YX, .Z = Rot.ZX});
            constexpr auto UnitY = Vector3({.X = Rot.XY, .Y = Rot.YY, .Z = Rot.ZY});
            constexpr auto UnitZ = Vector3({.X = Rot.XZ, .Y = Rot.YZ, .Z = Rot.ZZ});

            constexpr auto ExpectedUnitX = Q1.Rotate(Vector3::UNIT_X());
            constexpr auto ExpectedUnitY = Q1.Rotate(Vector3::UNIT_Y());
            constexpr auto ExpectedUnitZ = Q1.Rotate(Vector3::UNIT_Z());

            static_assert(IsVector3Near(UnitX, ExpectedUnitX, 1.0E-15));
            static_assert(IsVector3Near(UnitY, ExpectedUnitY, 1.0E-15));
            static_assert(IsVector3Near(UnitZ, ExpectedUnitZ, 1.0E-15));
        }        
    }   

    // Derivative
    {
        {    
            constexpr auto Q1Dot = Quaternion::IDENTITY().Derivative(Vector3::UNIT_X());
            constexpr auto Q2 = Quaternion{.X=0.5, .Y=0.0, .Z=0.0, .S=0.0};
            static_assert(Q1Dot == Q2);
        }

        {
            constexpr auto Q1Dot = Quaternion::IDENTITY().Derivative(Vector3::UNIT_Y());
            constexpr auto Q2 = Quaternion{.X=0.0, .Y=0.5, .Z=0.0, .S=0.0};
            static_assert(Q1Dot == Q2);
        }        

        { 
            constexpr auto Q1Dot = Quaternion::IDENTITY().Derivative(Vector3::UNIT_Z());
            constexpr auto Q2 = Quaternion{.X=0.0, .Y=0.0, .Z=0.5, .S=0.0};
            static_assert(Q1Dot == Q2);
        }        
                
        {
            static_assert(Quaternion::IDENTITY().Derivative(Vector3::ZERO()) == Quaternion::ZERO());
        }                

        // TODO: Supply more complex tests        
    }     

}