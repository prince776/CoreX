cmake_minimum_required(VERSION 3.0.0)
project(CoreX VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wall \
-Wextra \
-Wfloat-equal \
-Wundef \
-Wcast-align \
-Wwrite-strings \
-Wmissing-declarations \
-Wredundant-decls \
-Wshadow \
-Woverloaded-virtual \
")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Add the library source files
file(GLOB_RECURSE LIB_SOURCES "include/*.hpp")

# Add the library target
add_library(CoreX INTERFACE)
target_include_directories(CoreX INTERFACE ${LIB_SOURCES})

######### TESTING #########
# Add Google Test as a subdirectory
add_subdirectory(googletest)

# Enable testing
enable_testing()

# Collect test source files
file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get the test name by removing the file extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    message("Testname is : ${TEST_NAME}")
    # Add the test executable
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Link the test executable with Google Test and Corex library
    target_link_libraries(${TEST_NAME} gtest gtest_main CoreX)

    # Add test to CTest
    add_test(${TEST_NAME} ${TEST_NAME})
endforeach()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
