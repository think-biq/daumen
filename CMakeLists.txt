# 2021-∞ (c) blurryroots innovation qanat OÜ. All rights reserved.
# See license.md for details.

# Version 3.2 and upwards.
CMAKE_MINIMUM_REQUIRED (VERSION 3.2)

# Setup versioning.
set(BUILD_MAJOR 0)
set(BUILD_MINOR 6)
set(BUILD_PATCH 1)
set(BUILD_MICRO 8)
set(BUILD_VERSION "${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_PATCH}.${BUILD_MICRO}")

# Specify version for this project.
project (embrace VERSION ${BUILD_VERSION})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE PATH "" FORCE)
endif()

string(
    COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    IS_STANDALON_PROJECT
)

# .
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Make sure libraries will be places beside executable.
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (MSVC)
    # warning level 4 and all warnings as errors
    #add_compile_options(/W4 /WX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
    #add_definitions(-DHAVE_GETOPT_LONG=1)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wunused-result)
endif()

# Setup git submodules.
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Update submodules ...")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update \
                --init --recursive failed with ${GIT_SUBMOD_RESULT}, \
                please checkout submodules")
        endif()
    endif()
endif()

# Shared code library.
file(GLOB daumen_COMMON_SRC
    ${PROJECT_SOURCE_DIR}/src/daumen/*.c
)
if(BUILD_SHARED_LIBS)
add_library(daumen-common SHARED ${daumen_SRC})
else()
add_library(daumen-common ${daumen_SRC})
endif()
target_include_directories(daumen-common PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc/>
)
set_property(TARGET daumen-common PROPERTY C_STANDARD 11)

# Platform depending library.
if (WIN32)
    file(GLOB_RECURSE daumen_PLATFORM_SRC
        ${DAUMEN_PLATFORM_SRC}/src/daumen/win/*.c
    )
    message (STATUS "Adding windows platform source at: ${RAUM_PLATFORM_SRC}")
elseif (UNIX)
    message(FATAL_ERROR "MacOs currently no supported!")

    #file(GLOB_RECURSE RAUM_PLATFORM_SRC
    #    ${PROJECT_SOURCE_DIR}/src/raum/nix/*.c
    #)
    #message (STATUS "Adding *nix platform source at: ${RAUM_PLATFORM_SRC}")
else ()
    message (FATAL_ERROR "Cannot identify OS!")
endif ()
if(BUILD_SHARED_LIBS)
add_library(daumen-platform SHARED ${daumen_PLATFORM_SRC})
else()
add_library(daumen-platform ${daumen_PLATFORM_SRC})
endif()
target_include_directories(daumen-platform PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc/>
)
if(WIN32)
find_library(SHLWAPI Shlwapi.lib)
find_library(OLEAUT32 oleaut32.lib)
target_link_libraries(daumen-platform SHLWAPI OLEAUT32)
set_property(TARGET daumen-platform PROPERTY CXX_STANDARD 11)
else()
#
endif()
