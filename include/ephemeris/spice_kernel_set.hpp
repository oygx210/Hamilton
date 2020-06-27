#pragma once

#include <vector>
#include <string_view>
#include "ephemeris/spice_imports.hpp"

namespace Spice
{
    /* 
    * Scoped loading, unloading of Spice Kernels
    */
    class KernelSet
    {
    public:    
        // Default Constructor
        KernelSet() { }

        // Disable Copy Constructor
        KernelSet(const KernelSet& Other) = delete;

        // Load all when the kernel set is constructed
        // See C++ parameter packs for an explanation of the syntax
        template <typename... Args>
        KernelSet(const Args&... Kernels)
        {
            // Disable abort on error
            SpiceChar ErrorMode[] = "REPORT";
            erract_c("SET", sizeof(ErrorMode), ErrorMode);

            // Parameterically store each kernel string
            (mKernels.emplace_back(Kernels), ...);

            // Load each Kernel
            for(const auto& Kernel : mKernels)
            {
                furnsh_c(Kernel.data());
            }        
        }

        // Unload all when the kernel set goes out of scope
        ~KernelSet()
        {
            for(const auto& Kernel : mKernels)
            {
                unload_c(Kernel.data());
            }
        }
        
    private:
        std::vector<std::string_view> mKernels;
    };
}