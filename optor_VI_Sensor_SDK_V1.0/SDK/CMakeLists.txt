cmake_minimum_required(VERSION 2.8)

project(camtest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenCV REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(optorusbcam src/optorusb.cpp src/optorcam.cpp)
target_link_libraries(optorusbcam optorimu)

add_library(optorimu src/optorimu.cpp)

add_executable(camtest src/camtest.cpp)
target_link_libraries(camtest optorusbcam optorimu usb-1.0 ${OpenCV_LIBS})
