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

SET(CMAKE_COLOR_MAKEFILE ON)

# Hook into Geant4
find_package(Geant4 REQUIRED vis_opengl_x11)
include(${Geant4_USE_FILE})

# Hook into ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
#include_directories(${ROOT_INCLUDE_DIRS})


# -DHEPMC_DIR:STRING=/path/to/hepmc
include_directories(${HEPMC_DIR}/include)
file(GLOB hepmc ${HEPMC_DIR}/lib/libHepMC.so) #must be a better way....

# this package
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Some file lists
file(GLOB sources src/Cowbells/*.cc)
file(GLOB json_sources src/json/*.cpp)

# Copy over the python 
file(COPY python/ DESTINATION python/ FILES_MATCHING PATTERN "*.py")

# build library
add_library(cowbells SHARED ${sources})
#add_library(cowbells MODULE ${sources})
target_link_libraries(cowbells ${hepmc} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Tree Geom Graf Cintex json)

# JSON library
add_library(json SHARED ${json_sources})

# Build executable
#add_executable(test_cowbells_main main/test_cowbells_main.cc)
#target_link_libraries(test_cowbells_main cowbellsDict cowbells ${hepmc} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Tree Geom Graf Cintex)

add_executable(cowbells.exe main/cowbells.cc)
target_link_libraries(cowbells.exe cowbellsDict cowbells ${hepmc} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Tree Geom Graf Cintex)

### Generate dictionary

# rootcint dictionary - has trouble with G4 classes
#ROOT_GENERATE_DICTIONARY(cowbellsdict ${headers} LINKDEF dict/cowbell_linkdef.h OPTIONS -p)
#add_library(cowbellsDict MODULE cowbellsdict.cxx)
#target_link_libraries(cowbellsDict )

# reflex dictionary - handles C++ but don't forget an initial 
# import PyCintex
# somewhere early in your Python code
REFLEX_GENERATE_DICTIONARY(cowbells ${PROJECT_SOURCE_DIR}/dict/selection.h SELECTION dict/selection.xml )
#add_library(cowbellsDict MODULE cowbells_dict.cpp)
add_library(cowbellsDict SHARED cowbells_dict.cpp)
target_link_libraries(cowbellsDict cowbells ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Reflex)



