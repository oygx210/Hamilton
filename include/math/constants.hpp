#pragma once

#include "math/core_math.hpp"

/** 
 * @file constants.hpp 
 */

/// Speed of light in vacuum (m/s)
constexpr double SPEED_LIGHT = 299792458.0;

namespace Earth
{
    /// Earth mass (kg)    
    constexpr double MASS = 5.9722E24;                         

    /// Earth gravitational constant (m3/s2)    
    constexpr double GRAVITATIONAL_CONSTANT = 3.986004418E14;  

    /// Length of earth stellar day (s) as defined by the International
    /// Celestial Reference Frame
    constexpr double IERS_DAY_SECONDS = 86164.098903691;

    /// Earth equatorial rotational rate (rad/s)    
    constexpr double ROTATIONAL_RATE = 7.2921150E-5;       

    /// Earth standard gravity (m/s2)   
    constexpr double EQUATORIAL_GRAVITY = 9.7803253359;                

    namespace WGS84
    {
        /// Earth WGS84 Equatorial Radius (m)    
        constexpr double SEMI_MAJOR_AXIS = 6378137.0;          

        /// Earth WGS84 Polar Radius (m)            
        constexpr double SEMI_MINOR_AXIS = 6356752.314245;     

        /// Earth flattening (-)        
        constexpr double FLATTENING = 1.0 / 298.2572235630;  

        /// Earth eccentricity (-)        
        constexpr double ECCENTRICITY = 0.08181919084261345;   

        /// Earth Eccentricity Squared (-)        
        constexpr double ECCSQ = 1.0 - (SEMI_MINOR_AXIS / SEMI_MAJOR_AXIS) 
            * (SEMI_MINOR_AXIS / SEMI_MAJOR_AXIS); 
    }    
}