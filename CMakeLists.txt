# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.9)

# Set the project name
project (hyperv2hosts)

set (CMAKE_CXX_STANDARD 17)


include_directories(
        ${PROJECT_SOURCE_DIR}/3rd/fmt/include
	${PROJECT_SOURCE_DIR}/3rd/CLI11/include
)

# Add an executable
add_executable(hyperv2hosts hyperv2hosts.cpp)


