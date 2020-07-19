
add_library(HEphemerisLib "")

target_sources(HEphemerisLib
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/spice.cpp
)

# Link spice to the Ephemeris library
target_link_libraries(HEphemerisLib PRIVATE CppSpice)

# install(TARGETS HEphemerisLib CppSpice Spice
#         RUNTIME DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIR}/bin"
#         LIBRARY DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIR}/lib"
#         ARCHIVE DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIR}/lib/static")