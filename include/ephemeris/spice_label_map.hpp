#pragma once

#include <string_view>

namespace Spice
{
    /*
    * Maps a celestial object to it's spice ID
    */
    enum class ObjectID
    {
        INVALID = 0,
        MERCURY = 199,
        VENUS   = 299,
        EARTH   = 399,
        MOON    = 301,
        MARS    = 499,
        JUPITER = 599,
        SATURN  = 699,
        URANUS  = 799,
        NEPTUNE = 899,
        GATEWAY = -60000 // reference Lunar gateway NRHO
    };

    /* 
    * Return a string view of the Spice object ID
    */
    std::string_view GetObjectString(ObjectID ID);
}