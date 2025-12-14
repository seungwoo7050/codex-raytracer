cmake_minimum_required(VERSION 3.20)

project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(raytracer
    src/main.cpp
    src/ppm.cpp
    src/sphere.cpp
)

target_include_directories(raytracer PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_options(raytracer PRIVATE -Wall -Wextra -pedantic)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(unit_tests
    tests/unit/vec3_test.cpp
    tests/unit/sphere_test.cpp
    src/sphere.cpp
)

target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(unit_tests PRIVATE GTest::gtest_main)

gtest_discover_tests(unit_tests)

add_executable(integration_tests
    tests/integration/ppm_integration_test.cpp
    src/ppm.cpp
    src/sphere.cpp
)

target_include_directories(integration_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(integration_tests PRIVATE GTest::gtest_main)

gtest_discover_tests(integration_tests)
