#include "math/core_math.hpp"
#include "mission/orbital.hpp"
#include "math/constants.hpp"
#include "coordinates/earth.hpp"
#include "gtest/gtest.h"
#include "test_utils.hpp"

// Example taken from fundamentals of astrodynamics ad applications, 4th Edition
// David A. Vallado
// Example 2-5
TEST(Mission, OrbitalElements)
{
    constexpr auto Position = Vector3({6524834.0, 6862875.0, 6448296.0});
    constexpr auto Velocity = Vector3({4901.327, 5533.756, -1976.341});

    constexpr auto OrbitalElements = Newtonian2Kepler(Position, Velocity, EARTH::GRAVITATIONAL_CONSTANT);

    static_assert(IsNear(OrbitalElements.SemiParameter, 11067798.34266181663, 1.0E-15));
    static_assert(IsNear(OrbitalElements.SemiMajorAxis, 36127337.61967868358, 1.0E-15));
    static_assert(IsNear(OrbitalElements.Eccentricity, 0.8328533984875214902, 1.0E-15));
    static_assert(IsNear(OrbitalElements.Inclination, 87.86912617702644468, 1.0E-15));
    static_assert(IsNear(OrbitalElements.Node, 227.8982603572736991, 1.0E-15));
    static_assert(IsNear(OrbitalElements.ArgumentPerigee, 53.3849306184597765, 1.0E-15));
    static_assert(IsNear(OrbitalElements.TrueAnomoly, 92.3351567621373448, 1.0E-15));
    static_assert(IsNear(OrbitalElements.TrueLongitudeOfPeriapsis, 247.8064481974865032, 1.0E-15));
    static_assert(IsNear(OrbitalElements.ArgumentLatitude, 145.7200873805971355, 1.0E-15));
    static_assert(IsNear(OrbitalElements.TrueLongitude, 55.28270798147269005, 1.0E-15));
}