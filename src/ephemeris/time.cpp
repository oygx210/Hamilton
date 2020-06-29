#include "ephemeris/time.hpp"

double Spice::Date2Epoch(const std::string_view& Date)
{
    SpiceDouble EpochTime = 0.0;     
    str2et_c(reinterpret_cast<ConstSpiceChar*>(Date.data()), &EpochTime);
    return static_cast<double>(EpochTime);
}

std::string Spice::Epoch2TDB(double EpochTime)
{
    constexpr SpiceInt TIMESTRING_LENGTH = 51;    

    SpiceChar TimeChar[TIMESTRING_LENGTH];
    timout_c(static_cast<SpiceDouble>(EpochTime),
        "YYYY MON DD HR:MN:SC.### (TDB) ::TDB",
        TIMESTRING_LENGTH,
        TimeChar);
    
    return std::string(TimeChar);
}