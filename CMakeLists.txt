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
option(CHERRY_BUILD_TESTS "Build tests" ${CHERRY_TOP_LEVEL})
if(CHERRY_BUILD_TESTS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
        GIT_SHALLOW    ON
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
