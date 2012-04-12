# based on Geant4.9.5's B1 example

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(WCSim_WbLS)

# Hook into Geant4
find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Hook into ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(WCSim_WbLS WCSim_WbLS.cc ${sources} ${headers})
target_link_libraries(WCSim_WbLS ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} Tree)
