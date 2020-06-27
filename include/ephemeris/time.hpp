#pragma once

#include <string_view>
#include "spice_imports.hpp"

namespace Spice
{
    /**
     * Converts a date string to seconds in the current epoch
     */
    double Date2Epoch(const std::string_view& Date);
}