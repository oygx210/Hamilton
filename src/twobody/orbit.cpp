#include "twobody/orbit.hpp"

double TwoBody::Orbit::DeltaTimeFromTrueAnomoly(double TrueAnomoly) const noexcept
{
    // Orbital elements are invalid    
    if (mClassification == OrbitClassification::INVALID)
    {
        return Infinity<double>();
    }

    // Circular orbit
    if (IsCircular(mElements) == true)
    {
        return mMeanRadialPeriod * TrueAnomoly;
    }
    // Elliptical orbit
    if (IsClosed(mElements) == true)
    {
        const double DeltaTrueAnomoly = TrueAnomoly - mElements.TrueAnomoly;
        const double Polarity = Signum(DeltaTrueAnomoly);
        const double FullRevolutions = Floor(Polarity * DeltaTrueAnomoly / (2.0 * PI));

        const double TrueAnomolyEnd = TrueAnomoly - Polarity * FullRevolutions;
        const double AnomolyEnd = TrueToEccentricAnomoly(TrueAnomolyEnd, mElements.Eccentricity);
        const double MeanAnomolyEnd = TwoBody::EccentricToMeanAnomoly(AnomolyEnd, mElements.Eccentricity);

        return mMeanRadialPeriod * (2.0 * PI * FullRevolutions + (MeanAnomolyEnd - mMeanAnomoly) );
    }
    // Hyperbolic or parabolic trajectory
    else
    {
        // Check for valid hyperbolic range    
        if (mClassification == OrbitClassification::HYPERBOLIC)
        {
            const double CriticalAngle = Acos(1.0 / mElements.Eccentricity);
            if ((TrueAnomoly > PI - CriticalAngle) || (TrueAnomoly < -PI + CriticalAngle))
            {
                // Input anomoly is out of bounds    
                return Infinity<double>();
            }
        }

        const double AnomolyEnd = TrueToEccentricAnomoly(TrueAnomoly, mElements.Eccentricity);
        const double MeanAnomolyEnd = TwoBody::EccentricToMeanAnomoly(AnomolyEnd, mElements.Eccentricity);

        return mMeanRadialPeriod * ( MeanAnomolyEnd - mMeanAnomoly );                
    }
}

TwoBody::Orbit::Orbit(const KeplerianElements& Elements) noexcept : 
    mElements{Elements},
    mClassification{ClassifyOrbit(mElements)},
    mEccentricAnomoly{TwoBody::TrueToEccentricAnomoly(mElements.TrueAnomoly, mElements.Eccentricity)},
    mMeanRadialPeriod{TwoBody::CalculateMeanRadialPeriod(mElements)},
    mPeriod{(IsClosed(mElements) == true) ? 2.0 * PI * mMeanRadialPeriod : Infinity<double>()},
    mRadius{TwoBody::CalculateRadius(mElements)},
    mMeanAnomoly{TwoBody::EccentricToMeanAnomoly(mEccentricAnomoly, mElements.Eccentricity)},
    mDynamicIndex({
        .PtrMapDouble = {
            {"Semiparameter", &mElements.SemiParameter},
            {"SemiMajorAxis", &mElements.SemiMajorAxis},
            {"Eccentricity", &mElements.Eccentricity},
            {"Inclination", &mElements.Inclination},
            {"Node", &mElements.Node},
            {"TrueAnomoly", &mElements.TrueAnomoly},
            {"TrueLongitude", &mElements.TrueLongitude},
            {"TrueLongitudeOfPeriapsis", &mElements.TrueLongitudeOfPeriapsis},
            {"ArgumentLatitude", &mElements.ArgumentLatitude},
            {"ArgumentPerigee", &mElements.ArgumentPerigee},
            {"GravitationalParameter", &mElements.GravitationalParameter},
            {"Period", &mPeriod},
            {"EccentricAnomoly", &mEccentricAnomoly},
            {"MeanRadialPeriod", &mMeanRadialPeriod},
            {"Radius", &mRadius},
            {"MeanAnomoly", &mMeanAnomoly}
        }
    })
{
    
}

TwoBody::DeltaTimeAnomoly TwoBody::Orbit::AnomolyFromDeltaTime(double DeltaTime) const noexcept
{

    // Orbital elements are invalid    
    if (mClassification == OrbitClassification::INVALID)
    {
        return DeltaTimeAnomoly{};
    }
    // Circular orbit
    else if (IsCircular(mElements) == true)
    {
        return DeltaTimeAnomoly{
            .EccentricAnomoly = Fmod(DeltaTime, mPeriod),
            .NumberRevolutions = static_cast<int>(Floor(DeltaTime / mPeriod))
        };
    }
    // Elliptical orbit
    if (IsClosed(mElements) == true)
    {
        return DeltaTimeAnomoly{};
    }
    // Hyperbolic trajectory
    else if (mClassification == OrbitClassification::HYPERBOLIC)
    {
        return DeltaTimeAnomoly{};
    } 
    // Parabolic trajectory
    else
    {
        const double A = 1.5 * (DeltaTime / mMeanRadialPeriod - mMeanAnomoly);
        const double B = Cbrt(A + Sqrt(Square(A) + 1.0));
        const double EccentricAnomoly = 2.0 * Atan(B - 1.0 / B);
        return DeltaTimeAnomoly {
            .MeanAnomoly = EccentricToMeanAnomoly(EccentricAnomoly, mElements.Eccentricity),
            .EccentricAnomoly = EccentricAnomoly
        };
    }
}

void TwoBody::Orbit::Update(double DeltaTime) noexcept
{
    const auto Anomoly = AnomolyFromDeltaTime(DeltaTime);

    if (IsCircular(mElements) == true)
    {
        mElements.TrueLongitude = Anomoly.EccentricAnomoly;
    }
    else
    {
        mMeanAnomoly = Anomoly.MeanAnomoly;
        mEccentricAnomoly = Anomoly.EccentricAnomoly;
        mElements.TrueAnomoly = EccentricToTrueAnomoly(mEccentricAnomoly, mElements.Eccentricity);
        mRadius = CalculateRadius(mElements);
    }
}