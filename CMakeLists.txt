cmake_minimum_required(VERSION 3.10)
project(http_server)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

# Set C++ standard and compiler flags.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Add include directories.
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/usr/local/include/opencv5)

# Specify the directory for libraries.
link_directories(/usr/local/lib)

# Add the executable with source files.
add_executable(http_server 
    src/main.cpp
    src/http_server.cpp
    src/datastore.cpp
    src/ocrService.cpp
    src/setWorkDir.cpp
)

# Find and link against the Threads library.
find_package(Threads REQUIRED)

# Link against required libraries.
target_link_libraries(http_server
    Threads::Threads
    boost_system
    opencv_core
    opencv_imgproc
    opencv_highgui
    opencv_imgcodecs
    lept
    tesseract
)

# Set rpath to ensure the executable finds shared libraries at runtime.
set_target_properties(http_server PROPERTIES INSTALL_RPATH "/usr/local/lib")

# Option to enable tests.
option(BUILD_TESTS "Build the test suite" OFF)

if(BUILD_TESTS)
    enable_testing()

    # Add test executable for Criterion tests.
    add_executable(test_datastore
        tests/test_datastore.cpp
    )

    # Link test_datastore with the same libraries plus Criterion.
    target_link_libraries(test_datastore
        Threads::Threads
        boost_system
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_imgcodecs
        lept
        tesseract
        criterion
    )

    # Place the test executable in the same output directory.
    set_target_properties(test_datastore PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

    # Register the test executable with CTest.
    add_test(NAME DataStoreTests COMMAND test_datastore)
endif()