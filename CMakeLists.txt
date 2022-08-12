cmake_minimum_required(VERSION 3.10)
project(YZDN VERSION 1.0.0)

# Define global paths
set(YZDN_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")
set(YZDN_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src" CACHE INTERNAL "")
set(YZDN_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")
set(CosmoCore_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/cosmocore" CACHE INTERNAL "")
set(CosmoCore_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include/cosmocore" CACHE INTERNAL "")
set(CosmoPC_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/cosmopc" CACHE INTERNAL "")
string(REPLACE "/" "\\" YZDN_ROOT_DIR_WIN "${YZDN_ROOT_DIR}\\")

# Macro definitions
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Building Debug Version")
else()
    message("Building Release Version")
    set(RELEASE_BUILD 1 CACHE INTERNAL "")
    add_definitions("-DRELEASE_BUILD=${RELEASE_BUILD}")
endif()

# Flags
set(YZDN_FLAGS "-Wall" CACHE INTERNAL "")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND YZDN_FLAGS "-Wextra")
endif()

if (RELEASE_BUILD EQUAL 1)
    if (WIN32)
        list(APPEND YZDN_FLAGS "-O3" "-ffile-prefix-map=${YZDN_ROOT_DIR_WIN}=")
    else()
        list(APPEND YZDN_FLAGS "-O3" "-ffile-prefix-map=${YZDN_ROOT_DIR}=")
    endif()
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND YZDN_FLAGS "-Og" "-ggdb")
    else()
        list(APPEND YZDN_FLAGS "-Og" "-g3")
    endif()
endif()

# Build YZDN
include("${CosmoPC_SRC_DIR}/CMakeLists.txt")

# From https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#can-i-do-make-uninstall-with-cmake
# uninstall target
if (NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
