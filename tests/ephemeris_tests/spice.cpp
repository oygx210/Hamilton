#include "spice_kernel_set.hpp"
#include "spice_ephemeris.hpp"
#include "spice_label_map.hpp"
#include "spice_time.hpp"
#include "ephemeris/spice.hpp"
#include "test_utils.hpp"
#include "gtest/gtest.h"

#include <string_view>

TEST(Spice, GetEphemerisState)
{
    // Create ephemeris        
    auto GatewayEphemeris = SpiceEphemeris({
        .Object = Spice::GetObjectString(Spice::ObjectID::GATEWAY), 
        .Frame = Spice::GetFrameString(Spice::FrameID::J2000), 
        .Reference = Spice::GetObjectString(Spice::ObjectID::EARTH)
    });    

    {
        // Load Kernels    
        Spice::KernelSet Kernels{ };
        Kernels.LoadAuxillary(DEFAULT_LEAP_SECOND_KERNAL); // Leap seconds kernel
        Kernels.LoadEphemeris(std::string(PROJECT_DIR) 
            + std::string("/data/spice/gateway_nrho_reference/receding_horiz_3189_1burnApo_DiffCorr_15yr.bsp")); // Gateway
        
        // // Calculate initial epoch        
        double EpochTime = Spice::Date2Epoch("2024 June 10, 13:00:00 PST");

        EphemerisState Result = GatewayEphemeris.GetState(EpochTime);
        ASSERT_TRUE(IsVector3Near(Result.Pos, Vector3({-286826000.0, 264939000.0, 105314000.0}), 1.0E3));
        ASSERT_TRUE(IsVector3Near(Result.Vel, Vector3({-790.033, -394.054, -579.696}), 1.0E-3));
        ASSERT_NEAR(Result.LightTime, 1.34899, 1.0E-5);
    }
}