cmake_minimum_required(VERSION 3.10)
project(CosmoCore VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configure version
configure_file("${CosmoCore_INC_DIR}/cosmocore_version.hpp.in" "${CosmoCore_INC_DIR}/cosmocore_version.hpp")

# List of files to compile
set(CosmoCore_SRC_LIST
    "${CosmoCore_SRC_DIR}/cosmobus.cpp"
    "${CosmoCore_SRC_DIR}/cosmoclk.cpp"
    "${CosmoCore_SRC_DIR}/cosmocpu.cpp"
    "${CosmoCore_SRC_DIR}/cosmodsk.cpp"
    "${CosmoCore_SRC_DIR}/cosmokb.cpp"
    "${CosmoCore_SRC_DIR}/cosmomem.cpp"
    "${CosmoCore_SRC_DIR}/cosmoscr.cpp")

# Build shared and static
add_library(CosmoCore_static STATIC "${CosmoCore_SRC_LIST}")
add_library(CosmoCore_shared SHARED "${CosmoCore_SRC_LIST}")

# Includes
target_include_directories(CosmoCore_static PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${YZDN_INC_DIR}")
target_include_directories(CosmoCore_shared PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${YZDN_INC_DIR}")

# Common output name
set_target_properties(CosmoCore_static PROPERTIES OUTPUT_NAME cosmocore)
set_target_properties(CosmoCore_shared PROPERTIES OUTPUT_NAME cosmocore)

# Get SDL
find_package(SDL2 REQUIRED)

# Libraries
target_link_libraries(CosmoCore_static "${SDL2_LIBRARIES}" "SDL2_ttf")
target_link_libraries(CosmoCore_shared "${SDL2_LIBRARIES}" "SDL2_ttf")

# Flags
target_compile_options(CosmoCore_static PUBLIC "${YZDN_FLAGS}")
target_compile_options(CosmoCore_shared PUBLIC "${YZDN_FLAGS}")

# Install
install(TARGETS CosmoCore_static CosmoCore_shared
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(FILES
    "${CosmoCore_INC_DIR}/cosmobus.hpp"
    "${CosmoCore_INC_DIR}/cosmoclk.hpp"
    "${CosmoCore_INC_DIR}/cosmocore.hpp"
    "${CosmoCore_INC_DIR}/cosmocpu.hpp"
    "${CosmoCore_INC_DIR}/cosmodsk.hpp"
    "${CosmoCore_INC_DIR}/cosmokb.hpp"
    "${CosmoCore_INC_DIR}/cosmomem.hpp"
    "${CosmoCore_INC_DIR}/cosmoscr.hpp"
    "${CosmoCore_INC_DIR}/cosmocore_version.hpp"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cosmocore")