#include "ephemeris/spice_kernel_set.hpp"

#include "ephemeris/spice_imports.hpp"
#include "ephemeris/time.hpp"

// Weird archaic C magic
constexpr size_t MAX_OBJECTS = 1024;
constexpr size_t MAX_WINDOW_SIZE = 2000;

SPICEINT_CELL (SpiceIDs, MAX_OBJECTS);
SPICEDOUBLE_CELL (Coverage, MAX_WINDOW_SIZE);

std::string Spice::ObjectMetadata::PrettyString(void) const
{
    std::string Result = "Spice Object: " + std::to_string(ID) 
        + " Loaded in Kernel " + static_cast<std::string>(Kernel) + "\n";
    
    Result += "*** Coverage Summary ***\n";
    for (auto Index = 0; Index < Intervals.size(); ++Index)
    {
        Result += "Interval: " + std::to_string(Index) + "\n";
        Result += "Start: " + Epoch2TDB(Intervals[Index].Start) + "\n";
        Result += "Stop: " + Epoch2TDB(Intervals[Index].Stop) + "\n\n";
    }

    return Result;
}

Spice::KernelSet::KernelSet()
{
    // Disable abort on error
    SpiceChar ErrorMode[] = "REPORT";
    erract_c("SET", sizeof(ErrorMode), ErrorMode);
}

bool Spice::KernelSet::LoadEphemerisData(const std::string& Kernel)
{
    mKernels.emplace_back(Kernel);

    // Loads Kernel    
    furnsh_c(Kernel.data());

    // Introspect contents
    spkobj_c(Kernel.data(), &SpiceIDs);

    for (SpiceInt Index = 0; Index < card_c(&SpiceIDs); ++Index)
    {
        ObjectMetadata Meta;
        Meta.Kernel = Kernel;

        // Loads the object ID
        SpiceInt Object = SPICE_CELL_ELEM_I(&SpiceIDs, Index);
        Meta.ID = static_cast<int>(Object);

        // "Set Cardinality" of coverage, i.e clear it
        scard_c(0, &Coverage);

        // Find coverage window
        spkcov_c(Kernel.data(), Object, &Coverage);

        // Loop through each interval and get the endpoints in the reference epoch
        for (SpiceInt Interval = 0; Interval < wncard_c(&Coverage); ++Interval)
        {
            SpiceDouble Start, Stop;

            // Get start and stop times of this interval and store                        
            wnfetd_c(&Coverage, Interval, &Start, &Stop);
            Meta.Intervals.emplace_back(TimeInterval {.Start = static_cast<double>(Start), .Stop = static_cast<double>(Stop)});
        }

        // Store this object metadata
        mMeta.insert(std::make_pair(Meta.ID, std::move(Meta)));
    }    

    // TODO: Add error handling
    return true;
}

bool Spice::KernelSet::LoadAuxillaryData(const std::string& Kernel)
{
    mKernels.emplace_back(Kernel);

    // Loads Kernel    
    furnsh_c(Kernel.data());

    // TODO: Add error handling
    return true;
}

Spice::KernelSet::~KernelSet()
{
    for(const auto& Kernel : mKernels)
    {
        unload_c(Kernel.data());
    }
}