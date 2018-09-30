#----------------------------------------------------------------------------
# User settings
#
set(name G4SIM)
set(useROOT true)

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)

project(${name})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build ${name} with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT package
#
if(useROOT)
    find_package(ROOT)
    include_directories(${ROOT_INCLUDE_DIR})
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 and ROOT libraries
#
add_executable(${name} ${name}.cc ${sources} ${headers})
target_link_libraries(${name} ${Geant4_LIBRARIES})
if(useROOT)
    target_link_libraries(${name} ${ROOT_LIBRARIES})
    set (CMAKE_EXE_LINKER_FLAGS "-Wl,--no-as-needed")
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory
#
set(SCRIPTS
    macros/init.mac
    macros/vis.mac
    macros/gps.mac
    macros/config.mac
    macros/run.mac
    )
foreach(_script ${SCRIPTS})
    configure_file(
        ${PROJECT_SOURCE_DIR}/${_script}
        ${PROJECT_BINARY_DIR}
        COPYONLY
        )
endforeach()
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/G4out)

add_custom_target(main DEPENDS ${name})

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install (TARGETS ${name} DESTINATION bin)

