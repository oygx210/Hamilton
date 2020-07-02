#include "ephemeris/spice.hpp"

#include "spice_error.hpp"

#include <cstdio>

SpiceEphemeris::SpiceEphemeris(const Spice::EphemerisInputs& Inputs) noexcept :
    mParams{Inputs}
{

}

EphemerisState SpiceEphemeris::GetState(double EpochTime) const noexcept
{
    Spice::EphemerisState Intermediate = Spice::CalcEphemerisState(mParams, EpochTime);

    if (Intermediate.CalculationSuccess = false)    
    {
        // TODO: Handle Error
        puts(Spice::GetErrorAndReset().c_str());
        return EphemerisState{};
    }

    return EphemerisState{
        .Pos = Vector3({Intermediate.PosX * 1000.0, Intermediate.PosY * 1000.0, Intermediate.PosZ * 1000.0}),
        .Vel = Vector3({Intermediate.VelX * 1000.0, Intermediate.VelY * 1000.0, Intermediate.VelZ * 1000.0}),
        .LightTime = Intermediate.LightTime
    };
}