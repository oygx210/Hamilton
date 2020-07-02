## Overview

Hamilton is a hobby project GNC and 6DOF toolkit intended for space operations and simulation. Hamilton is written in C++20 and attempts to utilize modern C++ features for higher quality and more maintainable code, and a more ergonomic user experience. The long term objective of Hamilton is to be a viable candidate for use in real space operations.

Hamilton is named after Margaret Hamilton, lead flight software developer for the Apollo program.

## Features

#### Current

* Math types (`Vector3`, `Quaternion`, `Matrix3`, `Rotator` etc.) and corresponding operations.
* WGS84 Earth model
* Standard co-ordinate and navigation frames (`LLA`, `ECEF`, `ENU`, etc.). Celestial body independent calculations (partially complete).
* JPL NAIF Spice Ephemeris wrapper
* Constant evaluation context of core functionality
* Extensive test library utilising the GoogleTest framework
* Cross platform compatibility (partially tested)
* Azimuth, Elevation and Range calculations between two bodies

#### Planned
* Component based multi-body and subsystem simulation framework
* 6DOF dynamics model
* Aerodynamics model
* Extra-atmospheric drag models
* Atmospheric models for major solar system bodies
* Orbital mechanics mission planning toolkit
* Radiation pressure models
* Magnetic field models
* Post Newtonian GR models

## Requirements
* Requires a C++20 compliant compiler (Tested on GCC 9.3 and MSVC 16 2019)
* Default build system requires CMake >= 3.15

## Installing and Building
The below assumes you are using a unix like command line interface. Alternatively you can download the Windows CMake gui and use Microsoft Visual Studio as the generator. It is further assumed that you are using cmake.

* Clone the repo using the `--recurse-submodules` flag
* From the main project directory create a new build directory and navigate into it: `mkdir build && cd build`
* Configure using CMake `cmake .. -G <Generator>`, where <Generator> should be replaced with your generator of choice. Run `cmake -h` for a list of options
* Build with `cmake --build .`

#### Running the tests
Find `./HTestExec` (linux) or `./HTestExec.exe` (windows) in the build folder. This may be in different locations depending upon the platform, flags and compiler used. Run this exectuable to run the test suite.

## Usage
* Add `.../Hamilton/include` to your projects include path (using the full path to where Hamilton was cloned)
* Core libraries such as `math` and `ephemeris` are header only, and only need this include path to be added
* Other libraries such as `ephemeris` will create a shared or static library object which can be linked against.

## Development

Developer: Tom Reddell (tareddell@gmail.com)