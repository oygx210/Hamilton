#pragma once

/**
 * Container for ellipsoidal radii used in 
 * geodesic calculations
 * 
 * The azimuthal component is also known as the 
 * "prime vertical radius of curvature"
 */
struct EllipsoidRadii
{
    double Azimuthal = 0.0;
    double Inclined = 0.0;
};