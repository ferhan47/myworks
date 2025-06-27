cmake_minimum_required(VERSION 3.31)

project(medianfilter)

set(CMAKE_CXX_STANDARD 20)

# Burda brew kullandığım için set ile taşımam gerekti opencvyi
set(OpenCV_DIR /opt/homebrew/opt/opencv/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(medianfilter main.cpp)

target_link_libraries(medianfilter ${OpenCV_LIBS})