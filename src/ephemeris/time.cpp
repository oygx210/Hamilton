#include "ephemeris/time.hpp"

double Spice::Date2Epoch(const std::string_view& Date)
{
    SpiceDouble EpochTime = 0.0;     
    str2et_c(reinterpret_cast<ConstSpiceChar*>(Date.data()), &EpochTime);
    return static_cast<double>(EpochTime);
}