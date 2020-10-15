# cmake needs this line
cmake_minimum_required(VERSION 2.8)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -std=c++11")

# Define project name
project(task2_6)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)
find_package (Threads) 
find_package(Boost COMPONENTS filesystem regex REQUIRED)  
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(${Boost_INCLUDE_DIRS})

# Declare the executable target built from your sources
add_executable(task2_6 task2_6.cpp)

# Link your application with OpenCV libraries
aux_source_directory(. DIR_SRCS)
target_link_libraries(task2_6 -lboost_system ${OpenCV_LIBS} ${Boost_LIBRARIES})
target_link_libraries (task2_6 ${CMAKE_THREAD_LIBS_INIT})
