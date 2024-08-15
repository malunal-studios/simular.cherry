set(CHERRY_INTERNAL_LIBRARY cherry_interface)
add_library(${CHERRY_INTERNAL_LIBRARY} INTERFACE)
add_library(simular::cherry ALIAS ${CHERRY_INTERNAL_LIBRARY})
target_compile_options(
    ${CHERRY_INTERNAL_LIBRARY}
    INTERFACE -Wall
    INTERFACE -Wextra
    INTERFACE -Wpedantic
    INTERFACE -Wno-switch
)

target_include_directories(
    ${CHERRY_INTERNAL_LIBRARY}
    INTERFACE ${PROJECT_SOURCE_DIR}/source
)

target_link_libraries(
    ${CHERRY_INTERNAL_LIBRARY}
    INTERFACE simular::allocators
    INTERFACE simular::tooling
)

if(CHERRY_ENABLE_PROFILING)
    target_compile_definitions(
        ${CHERRY_INTERNAL_LIBRARY}
        INTERFACE SIMULAR_TOOLING_ENABLE_MACROS
    )
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE entry.cpp)
