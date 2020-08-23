#pragma once

/**
 * Simple container for a geodetic latitude, longitude altitude type
 */
struct LLA
{
    double Lat = 0.0; // latitude (deg)
    double Lgt = 0.0; // longitude (deg)
    double Alt = 0.0; // altitude (m)
};

/**
 * Simple container for a geodetic latitude, longitude altitude type
 * in radians
 */
struct LLARad
{
    double LatRad = 0.0; // latitude (rad)
    double LgtRad = 0.0; // longitude (rad)
    double Alt = 0.0; // altitude (m)
};