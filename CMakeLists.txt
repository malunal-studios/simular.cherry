cmake_minimum_required(VERSION 3.16)
project(cherry VERSION 1.0.0 LANGUAGES CXX)

set(CHERRY_TOP_LEVEL OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CHERRY_TOP_LEVEL ON)
endif()

include(FetchContent)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch tooling
FetchContent_Declare(
    simular_tooling
    GIT_REPOSITORY https://github.com/simular-technologies/simular.tooling.git
    GIT_TAG        v1.0.2
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(simular_tooling)

option(CHERRY_ENABLE_PROFILING "Profiles Cherry" OFF)
add_subdirectory(source)

# Do you want to build the tests?
option(CHERRY_BUILD_TESTS "Build tests" OFF)
if(CHERRY_BUILD_TESTS)
    message(STATUS "Tests compilation enabled for Cherry")
    message(STATUS "Fetching Google Test library...")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
        GIT_SHALLOW    ON
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    message(STATUS "Fetching Google Test library - done")

    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

# Do you want to build the benchmarks?
option(CHERRY_BUILD_BENCHMARKS "Build benchmarks" OFF)
if(CHERRY_BUILD_BENCHMARKS)
    # Benchmarks should always run in release mode so capture old build type,
    # configure the benchmarks then reset the build type after.
    set(OLD_BUILD_TYPE ${CMAKE_BUILD_TYPE})
    set(CMAKE_BUILD_TYPE "Release")

    message(STATUS "Benchmarks compilation enabled for Cherry")
    message(STATUS "Fetching Google Benchmark library...")
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        v1.8.5
        GIT_SHALLOW    ON
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googlebenchmark)
    message(STATUS "Fetching Google Benchmark library - done")

    add_subdirectory(benchmarks)

    # Reset build type.
    set(CMAKE_BUILD_TYPE ${OLD_BUILD_TYPE})
endif()