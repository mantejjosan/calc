# Minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(calc VERSION 0.2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable for the main program
add_executable(calc calc.cpp)

# Include yaml-cpp
include_directories($ENV{PREFIX}/include)
target_link_libraries(calc PRIVATE $ENV{PREFIX}/lib/libyaml-cpp.a)

# Enable testing
enable_testing()

# Read test cases from file
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/test_cases.txt TEST_CASES_CONTENTS)
string(REPLACE "\n" ";" TEST_CASES_LIST ${TEST_CASES_CONTENTS})

# Loop through each test case and add as a test
foreach(TEST_CASE ${TEST_CASES_LIST})
    # Replace spaces in the test case to create a unique name
    string(REPLACE " " "_" TEST_NAME ${TEST_CASE})
    string(REPLACE "+" "plus" TEST_NAME ${TEST_NAME})
    string(REPLACE "-" "minus" TEST_NAME ${TEST_NAME})
    string(REPLACE "*" "mult" TEST_NAME ${TEST_NAME})
    string(REPLACE "/" "div" TEST_NAME ${TEST_NAME})
    string(REPLACE "(" "open" TEST_NAME ${TEST_NAME})
    string(REPLACE ")" "close" TEST_NAME ${TEST_NAME})

    # Add the test, running the calc program with each test case input
    add_test(NAME Test_${TEST_NAME}
             COMMAND ${PROJECT_NAME} ${TEST_CASE}
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(Test_${TEST_NAME} PROPERTIES PASS_REGULAR_EXPRESSION "Result:")
endforeach()
