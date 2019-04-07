cmake_minimum_required(VERSION 3.13)

project(fastcode)

##### Setting up the CXX flags #####
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} -std=c++11 -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEFAULT} -std=c11 -Wall -Wextra")

##### Using FindCriterion.cmake we find the package ######
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/")
find_package(Criterion REQUIRED)

##### Include header files #####
include_directories(
        include
)

##### Source files to compile for benchmark executable #####
add_executable(benchmark
        src/benchmark.cpp
        src/run_benchmark.cpp
        src/cpp_utils.cpp
        src/penguin.c
        src/hgwosca.c
        src/objectives.c
        src/utils.c)

##### Integration test executable ######
add_executable(test_integration
        tests/test_integration.cpp
        src/hgwosca.c
        src/objectives.c
        src/utils.c
        src/cpp_utils.cpp)
target_include_directories(test_integration PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_integration PRIVATE ${CRITERION_LIBRARIES})

##### penguin unit test ######
add_executable(test_hgwosca
        tests/test_hgwosca.cpp
        src/hgwosca.c
        src/utils.c
        src/objectives.c
        src/cpp_utils.cpp)
target_include_directories(test_hgwosca PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_hgwosca PRIVATE ${CRITERION_LIBRARIES})


##### penguin unit test ######
add_executable(test_penguin
        tests/test_penguin.cpp
        src/penguin.c
        src/utils.c
        src/cpp_utils.cpp)
target_include_directories(test_penguin PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_penguin PRIVATE ${CRITERION_LIBRARIES} m)

##### objectives unit test ######
add_executable(test_objectives
        tests/test_objectives.cpp
        src/objectives.c
        src/utils.c
        src/cpp_utils.cpp)
target_include_directories(test_objectives PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_objectives PRIVATE ${CRITERION_LIBRARIES})


##### All unit tests together #####
add_executable(test_units
        tests/test_objectives.cpp
        tests/test_hgwosca.cpp
        tests/test_penguin.cpp
        tests/test_utils.cpp
        src/hgwosca.c
        src/penguin.c
        src/objectives.c
        src/utils.c
        src/cpp_utils.cpp)
target_include_directories(test_units PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_units PRIVATE ${CRITERION_LIBRARIES})