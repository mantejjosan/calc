# Minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(calc VERSION 0.2 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable for the main program
add_executable(calc calc.cpp)

# Variables for YAML-CPP
set(YAML_CPP_VERSION "0.7.0")
set(YAML_CPP_URL "https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-${YAML_CPP_VERSION}.tar.gz")
set(YAML_CPP_SRC_DIR "${CMAKE_BINARY_DIR}/yaml-cpp")
set(YAML_CPP_LIB_DIR "${YAML_CPP_SRC_DIR}/build")
set(YAML_CPP_LIB_FILE "${YAML_CPP_LIB_DIR}/libyaml-cpp.a")

# Check if YAML-CPP exists; if not, download and build
if(NOT EXISTS ${YAML_CPP_LIB_FILE})
    message(STATUS "YAML-CPP not found. Downloading and building...")
    
    # Download YAML-CPP
    file(DOWNLOAD ${YAML_CPP_URL} ${CMAKE_BINARY_DIR}/yaml-cpp.tar.gz 
         SHOW_PROGRESS
         STATUS DOWNLOAD_STATUS
         TLS_VERIFY ON)
    
    list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if(NOT STATUS_CODE EQUAL 0)
        message(FATAL_ERROR "Failed to download YAML-CPP")
    endif()
    
    # Extract YAML-CPP archive
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf yaml-cpp.tar.gz
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        RESULT_VARIABLE EXTRACT_RESULT
    )
    if(NOT EXTRACT_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to extract YAML-CPP")
    endif()
    
    file(RENAME ${CMAKE_BINARY_DIR}/yaml-cpp-yaml-cpp-${YAML_CPP_VERSION} ${YAML_CPP_SRC_DIR})
    
    # Build YAML-CPP
    execute_process(
        COMMAND ${CMAKE_COMMAND} -S ${YAML_CPP_SRC_DIR} -B ${YAML_CPP_LIB_DIR}
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            -DYAML_CPP_BUILD_TESTS=OFF
            -DYAML_CPP_BUILD_TOOLS=OFF
        RESULT_VARIABLE CONFIG_RESULT
    )
    if(NOT CONFIG_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to configure YAML-CPP")
    endif()
    
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${YAML_CPP_LIB_DIR}
        RESULT_VARIABLE BUILD_RESULT
    )
    if(NOT BUILD_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to build YAML-CPP")
    endif()
endif()

# Include and link YAML-CPP
target_include_directories(calc PRIVATE ${YAML_CPP_SRC_DIR}/include)
target_link_libraries(calc PRIVATE ${YAML_CPP_LIB_FILE})

# Enable testing
enable_testing()

# Check if test_cases.txt exists
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test_cases.txt")
    message(FATAL_ERROR "test_cases.txt not found")
endif()

# Read test cases from file
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/test_cases.txt TEST_CASES_CONTENTS)
string(REPLACE "\n" ";" TEST_CASES_LIST ${TEST_CASES_CONTENTS})

# Loop through each test case and add as a test
foreach(TEST_CASE ${TEST_CASES_LIST})
    if(NOT TEST_CASE STREQUAL "")
        # Replace spaces and special characters in the test case to create a unique name
        string(REPLACE " " "_" TEST_NAME ${TEST_CASE})
        string(REPLACE "+" "plus" TEST_NAME ${TEST_NAME})
        string(REPLACE "-" "minus" TEST_NAME ${TEST_NAME})
        string(REPLACE "*" "mult" TEST_NAME ${TEST_NAME})
        string(REPLACE "/" "div" TEST_NAME ${TEST_NAME})
        string(REPLACE "(" "open" TEST_NAME ${TEST_NAME})
        string(REPLACE ")" "close" TEST_NAME ${TEST_NAME})
        
        # Add the test
        add_test(NAME Test_${TEST_NAME}
                 COMMAND $<TARGET_FILE:calc> ${TEST_CASE}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        set_tests_properties(Test_${TEST_NAME} 
            PROPERTIES 
            PASS_REGULAR_EXPRESSION "Result:"
            TIMEOUT 30)
    endif()
endforeach()
