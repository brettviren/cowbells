# based on Geant4.9.5's B1 example

# usage:
#
#  This file is at /path/to/cowbells/CMakeLists.txt
#
# mkdir cowbells-build
# cd cowbells-buid
# cmake -DROOT_DIR=/path/to/root -DGeant4_DIR=/path/to/geant4 /path/to/cowbells
# make

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(cowbells)

# Hook into Geant4
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

# Hook into ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
#include_directories(${ROOT_INCLUDE_DIRS})

# What about G4VMC?

# this package
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Some file lists
file(GLOB sources src/Cow*.cxx)
file(GLOB headers inc/Cow*.h inc/units.h)

# Copy over the python 
file(COPY python/ DESTINATION python/ FILES_MATCHING PATTERN "*.py")

# Generate dictionary
ROOT_GENERATE_DICTIONARY(cowbellsdict ${headers} LINKDEF dict/cowbell_linkdef.h OPTIONS -p)

# build library
add_library(cowbells MODULE ${sources} cowbellsdict.cxx)
target_link_libraries(cowbells ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Tree)
