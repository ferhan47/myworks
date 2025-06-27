cmake_minimum_required(VERSION 3.31)

project(untitled)

set(CMAKE_CXX_STANDARD 20)

# Eğer OpenCV'yi Homebrew ile yüklediysen, OpenCV_DIR yolunu ayarla (macOS M1 için örnek)
set(OpenCV_DIR /opt/homebrew/opt/opencv/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(untitled main.cpp)

target_link_libraries(untitled ${OpenCV_LIBS})
