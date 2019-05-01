cmake_minimum_required(VERSION 3.9.2)

project(fastcode)

##### Setting up the CXX flags #####
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} -std=c++11 -Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEFAULT} -std=c11 -Wall -Wextra")

##### DEBUG #####
OPTION(DEBUG "DEBUG flag" OFF)
IF(DEBUG)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF(DEBUG)

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
        src/pso.c
        src/squirrel.c
        src/objectives.c
        src/utils.c)


##### hgwosca integration test executable ######
add_executable(test_integration_hgwosca
        tests/test_integration_hgwosca.c
        tests/utils_test.c
        src/hgwosca.c
        src/objectives.c
        src/utils.c)
target_include_directories(test_integration_hgwosca PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_integration_hgwosca
        PRIVATE ${CRITERION_LIBRARIES}
        m)

##### hgwosca unit test ######
add_executable(test_hgwosca
        tests/test_hgwosca.c
        src/cpp_utils.cpp
        src/hgwosca.c
        src/utils.c
        src/objectives.c)
target_include_directories(test_hgwosca PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_hgwosca PRIVATE ${CRITERION_LIBRARIES})


##### pso basic integration test executable  ######
add_executable(test_integration_pso
        tests/test_integration_pso.c
        tests/utils_test.c
        src/pso.c
        src/objectives.c
        src/utils.c)
target_include_directories(test_integration_pso PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_integration_pso
        PRIVATE ${CRITERION_LIBRARIES}
        m)

##### pso basic unit test ######
add_executable(test_pso
        tests/test_pso.c
        src/cpp_utils.cpp
        src/pso.c
        src/utils.c
        src/objectives.c)
target_include_directories(test_pso PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_pso PRIVATE ${CRITERION_LIBRARIES})

##### squirrel integration test executable  ######
add_executable(test_integration_squirrel
        tests/test_integration_squirrel.c
        tests/utils_test.c
        src/squirrel.c
        src/objectives.c
        src/utils.c)
target_include_directories(test_integration_squirrel PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_integration_squirrel
        PRIVATE ${CRITERION_LIBRARIES}
        m)

##### squirrel unit test ######
add_executable(test_squirrel
        tests/test_squirrel.c
        src/cpp_utils.cpp
        src/squirrel.c
        src/utils.c
        src/objectives.c)
target_include_directories(test_squirrel PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_squirrel PRIVATE ${CRITERION_LIBRARIES})

##### Penguin integration test executable ######
add_executable(test_integration_pengu
       tests/test_integration_pengu.c
       tests/utils_test.c
       src/penguin.c
       src/objectives.c
       src/utils.c)
target_include_directories(test_integration_pengu PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_integration_pengu
       PRIVATE ${CRITERION_LIBRARIES}
       m)

##### penguin unit test ######
add_executable(test_penguin
        tests/test_penguin.c
        src/cpp_utils.cpp
        src/penguin.c
        src/utils.c
        src/objectives.c)
target_include_directories(test_penguin PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_penguin PRIVATE ${CRITERION_LIBRARIES} m)


##### objectives unit test ######
add_executable(test_objectives
        tests/test_objectives.c
        src/cpp_utils.cpp
        src/objectives.c
        src/utils.c)
target_include_directories(test_objectives PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_objectives PRIVATE ${CRITERION_LIBRARIES})

##### objectives unit test ######
add_executable(test_utils
        tests/test_utils.c
        src/utils.c)
target_include_directories(test_utils PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_utils PRIVATE ${CRITERION_LIBRARIES})



##### All unit tests together #####
add_executable(test_units
        tests/test_objectives.c
        tests/test_hgwosca.c
        tests/test_penguin.c
        tests/test_squirrel.c
        tests/test_pso.c
        tests/test_cpp_utils.cpp
        tests/test_utils.c
        src/cpp_utils.cpp
        src/hgwosca.c
        src/penguin.c
        src/squirrel.c
        src/pso.c
        src/objectives.c
        src/utils.c
        src/utils.c)
target_include_directories(test_units PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(test_units PRIVATE ${CRITERION_LIBRARIES})

enable_testing()
add_test(unit
  test_units -q)
add_test(integration_hgwosca
  test_integration_hgwosca -q)
add_test(integration_pengu
  test_integration_pengu -q)
add_test(integration_pso
  test_integration_pso -q)
add_test(integration_squirrel
  test_integration_squirrel -q)
