#include "ephemeris/spice_label_map.hpp"

#include <cstdio>

std::string_view Spice::GetObjectString(ObjectID ID)
{
    switch (ID)
    {
        case ObjectID::GATEWAY: return std::string_view("-60000");
        case ObjectID::EARTH  : return std::string_view("399");
        case ObjectID::MOON   : return std::string_view("301");
        case ObjectID::MARS   : return std::string_view("499");
        case ObjectID::JUPITER: return std::string_view("599");
        default:
        {
            printf("Invalid ObjectID %u\n", ID);
            return std::string_view("INVALID");
        }
    }
}