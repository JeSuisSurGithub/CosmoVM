cmake_minimum_required(VERSION 3.10)
project(CosmoPC VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configure version
configure_file("${CosmoPC_SRC_DIR}/cosmopc_version.hpp.in" "${CosmoPC_SRC_DIR}/cosmopc_version.hpp")

# Build libraries
include("${CosmoCore_SRC_DIR}/CMakeLists.txt")

# Source files
add_executable(CosmoPC
    "${CosmoPC_SRC_DIR}/cosmoasm.cpp"
    "${CosmoPC_SRC_DIR}/main.cpp")

# Includes
target_include_directories(CosmoPC PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CosmoPC_SRC_DIR}"
    "${YZDN_INC_DIR}")

# Lower case name
set_target_properties(CosmoPC PROPERTIES OUTPUT_NAME cosmopc)

# Get SDL2
find_package(SDL2 REQUIRED)

# Link built libraries
target_link_directories(CosmoPC PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_BINARY_DIR}")

# Link with static one
target_link_libraries(CosmoPC PUBLIC CosmoCore_static "${SDL2_LIBRARIES}" "SDL2_ttf")

# Flags
target_compile_options(CosmoPC PUBLIC "${YZDN_FLAGS}")

# Wait first for libraries to build
add_dependencies(CosmoPC CosmoCore_static CosmoCore_shared)