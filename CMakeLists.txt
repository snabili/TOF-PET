#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.27)
project(TOF_PET)

find_package(Geant4 REQUIRED ui_all vis_all)
#include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(tof_pet tof_pet.cc ${sources} ${headers})
target_include_directories(tof_pet PUBLIC include)
target_link_libraries(tof_pet ${Geant4_LIBRARIES})
