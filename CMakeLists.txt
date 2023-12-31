# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Calorimeter_AMS)

# Find Geant4 package, activating all available UI and Vis drivers by default
find_package(Geant4 REQUIRED ui_all vis_all)

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable, and link it to the Geant4 libraries
add_executable(calorimeter calorimeter.cc ${sources} ${headers})
target_link_libraries(calorimeter ${Geant4_LIBRARIES})

# For internal Geant4 use
add_custom_target(Calorimeter_AMS DEPENDS calorimeter)
