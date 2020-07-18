
add_library(HDisturbancesLib "")

target_sources(HDisturbancesLib
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/earth_gravity.cpp
)