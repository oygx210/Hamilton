#include "ephemeris/spice_ephemeris.hpp"

SpiceEphemeris::SpiceEphemeris(const std::string& Object, 
                               const std::string& Frame, 
                               const std::string& Reference) :
    mObjectID{Object},
    mFrame{Frame},
    mReference{Reference}
{
}

SpiceEphemeris::SpiceEphemeris() : 
    mObjectID{"INVALID"}
{
}

EphemerisState SpiceEphemeris::GetState(double EpochTime) const
{
    std::vector<SpiceDouble> Buffer(6);
    SpiceDouble LightTimeSeconds = 0.0;

    // Update ephemeris at the given epoch time using spice
    spkezr_c(static_cast<ConstSpiceChar*>(mObjectID.data()), 
             static_cast<SpiceDouble>(EpochTime), 
             static_cast<ConstSpiceChar*>(mFrame.data()), 
             "NONE", 
             static_cast<ConstSpiceChar*>(mReference.data()), 
             Buffer.data(), 
             &LightTimeSeconds);

    // Return in a more modern format
    return EphemerisState{
        .Pos = Vector3({static_cast<double>(Buffer[0]) * 1000.0, static_cast<double>(Buffer[1]) * 1000.0, static_cast<double>(Buffer[2]) * 1000.0}),
        .Vel = Vector3({static_cast<double>(Buffer[3]) * 1000.0, static_cast<double>(Buffer[4]) * 1000.0, static_cast<double>(Buffer[5]) * 1000.0}),
        .LightTime = static_cast<double>(LightTimeSeconds)
    };    
}