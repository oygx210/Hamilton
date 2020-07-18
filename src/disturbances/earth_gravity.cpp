#include "disturbances/earth_gravity.hpp"

#include "math/constants.hpp"

template <HarmonicOrder Order>
void EarthGravity<Order>::Update(const Spherical& Sph, const TrigComponents& Trig)
{
    double AccelerationRadial = -EARTH::GRAVITATIONAL_CONSTANT / Sph.Rad;
    double AccelerationAzimuth = 0.0;

    if constexpr (Order != HarmonicOrder::SPHERICAL)
    {
        // Calculate radius^4 (km^4)
        double Radius = Sph.Rad * 1.0E-3;
        double RadiusQuart = Radius;
        RadiusQuart *= RadiusQuart;
        RadiusQuart *= RadiusQuart;
        double SinAzm2 = Trig.SinAzm * Trig.SinAzm;
    
        AccelerationAzimuth -= J2 * 3.0 * Trig.CosAzm * Trig.SinAzm / RadiusQuart * 1.0E3;
        AccelerationRadial += J2 * 1.5 * (3.0 * SinAzm2 - 1.0) / RadiusQuart * 1.0E3;

        if constexpr (Order == HarmonicOrder::THIRD)
        {
            double RadiusPent = RadiusQuart * Radius;
            AccelerationAzimuth -= J3 * 1.5 * Trig.CosAzm * (5.0 * SinAzm2 - 1.0) / RadiusPent * 1.0E3;
            AccelerationRadial += J3 * 2.0 * (5.0 * SinAzm2 - 3.0) / RadiusPent * 1.0E3;
        }        
    }

    mAcceleration = Vector3(AccelerationRadial, AccelerationAzimuth, 0.0);
}