#pragma once

#include "kepler.hpp"
#include "meta/indexable.hpp"

namespace TwoBody
{
    /** 
     * Anomoly resulting from propogating an orbit ahead by some delta time
     */
    struct DeltaTimeAnomoly
    {
        double MeanAnomoly = 0.0;
        double EccentricAnomoly = 0.0;
        int32_t NumberRevolutions = 0;
    };

    /** 
     * Objects which stores and mutates an orbital state (as keplerian elements)    
     */
    class Orbit
    {
    public:    

        /** 
         * Instantiate from Keplerian elements
         * @param Elements Keplerian Elements
         * @return Orbit
         */
        static Orbit FromKeplerianElements(const KeplerianElements& Elements) noexcept
        {
            return Orbit(Elements);
        }

        /** 
         * Instantiate from Newtonian state
         * @param Position (x, y, z) (m)
         * @param Velocity (x, y ,z) (m/s)
         * @param GravitationalParameter Central body gravitational parameter (m3/s2)
         * @return Orbit
         */
        static Orbit FromNewtonian(const Vector3& Position, const Vector3& Velocity, double GravitationalParameter) noexcept
        {
            return Orbit(Newtonian2Kepler(Position, Velocity, GravitationalParameter));
        }

        /** 
         * @return KeplerianElements
         */
        const KeplerianElements& GetElements(void) const noexcept {return mElements;}

        /**
         * @return Eccentric/Hyperbolic/Parabolic anomoly depending upon orbit classification
         */
        double GetEccentricAnomoly(void) const noexcept {return mEccentricAnomoly;}

        /**
         * @return Mean time to travel one radian (s)
         */
        double GetMeanRadialPeriod(void) const noexcept {return mMeanRadialPeriod;}        

        /**
         * @return Orbital period (s)
         */
        double GetPeriod(void) const noexcept {return mPeriod;}

        /**
         * @return Current radius from centre of central body (m)
         */
        double GetRadius(void) const noexcept {return mRadius;}

        /**
         * @return Current mean anomoly
         */
        double GetMeanAnomoly(void) const noexcept {return mMeanAnomoly;}

        /** 
         * @return Orbit classification  
         */
        OrbitClassification GetClassification(void) const noexcept {return mClassification;}
 
        /** 
         * Calculates the delta time required to reach a given `TrueAnomoly`. Can calculate past states if `TrueAnomoly` < Current 
         * True Anomoly or `TrueAnomoly` < 0. Hence, must add 2*PI to the desired anomoly if referring to the state less than one full 
         * revolution in the future but such that `TrueAnomoly` < Current True Anomoly         
         * 
         * @param TrueAnomoly The desired true anomoly (rad), can be positive or negative.
         * @return Delta Time to the given delta anomoly, a negative time indicates that the anomoly was reached in the past
         */
        double DeltaTimeFromTrueAnomoly(double TrueAnomoly) const noexcept;

        /** 
         * Calculates the new orbital anomoly as a result of a delta time from the current orbital state
         * @param DeltaTime Time difference from current orbital state
         * @return Anomoly result
         */
        DeltaTimeAnomoly AnomolyFromDeltaTime(double DeltaTime) const noexcept;

        /** 
         * Updates the orbital parameters in place to a new state `DeltaTime` apart from
         *  the current orbital state
         * @param DeltaTime Time difference from current orbital state
         */
        void Update(double DeltaTime) noexcept;

        /** 
         * @return const reference to the dynamic index map
         */
        const Indexable* GetDynamicIndexMap(void) const {return &mDynamicIndex;}

    private:

        // Current keplerian state
        KeplerianElements mElements{};

        // Classification of the conic section
        OrbitClassification mClassification = OrbitClassification::INVALID;        

        // rad
        double mEccentricAnomoly = 0.0;

        // Mean time to travel one radian (s)
        double mMeanRadialPeriod = 0.0;        

        // Orbital period (s)
        double mPeriod = 0.0;

        // Current radius from centre of central body (m)
        double mRadius = 0.0;

        // rad
        double mMeanAnomoly = 0.0;    

        // key to pointer mappings
        Indexable mDynamicIndex;          

        Orbit(const KeplerianElements& Elements) noexcept;
    };
}    