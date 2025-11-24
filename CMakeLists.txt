cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/starm-clang.cmake)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME engineer)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
# Add CMSIS-DSP
set(CMSISCORE "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS")
add_subdirectory(CMSIS-LIB/CMSIS-DSP/Source bin_dsp)


# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

file(GLOB_RECURSE THREAD
        "Project/CallBack/*.c" "Project/CallBack/*.cpp"
        "Project/App/*.c" "Project/App/*.cpp"
        "Project/Bsp/*.c" "Project/Bsp/*.cpp"
        "Project/Mod/*.c" "Project/Mod/*.cpp"
        "Project/Obj/*.c" "Project/Obj/*.cpp"
        "Project/OS/*.c" "Project/OS/*.cpp"
)
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
        ${THREAD}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
        "Project/App"
        "Project/App/Imu/BMI088/inc"
        "Project/Bsp"
        "Project/Mod"
        "Project/Obj"
        "Project/OS"
        "../"
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    CMSISDSP
    # Add user defined libraries
)



get_property(targets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
foreach(t ${targets})
    get_target_property(opts ${t} COMPILE_OPTIONS)
    get_target_property(flags ${t} COMPILE_FLAGS)
    get_target_property(defs ${t} COMPILE_DEFINITIONS)
    message(STATUS "Target: ${t}")
    message(STATUS "  COMPILE_OPTIONS = ${opts}")
    message(STATUS "  COMPILE_FLAGS   = ${flags}")
    message(STATUS "  COMPILE_DEFINITIONS = ${defs}")
    message(STATUS "  CXX_FLAGS (global) = ${CMAKE_CXX_FLAGS}")
    message(STATUS "  CXX_FLAGS_${CMAKE_BUILD_TYPE} = ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "  C_FLAGS_${CMAKE_BUILD_TYPE} = ${CMAKE_C_FLAGS_RELEASE}")
endforeach()