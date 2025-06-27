cmake_minimum_required(VERSION 3.18)
project(deneme123 LANGUAGES CXX CUDA)

# OpenCV'yi bul
find_package(OpenCV REQUIRED)

# Derleme seçenekleri
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CUDA_STANDARD 14)

# CUDA ile derlemede şu satır önemli
enable_language(CUDA)

# Executable'ı tanımla
add_executable(deneme123 deneme123.cpp)

# OpenCV ve CUDA'yı bağla
target_link_libraries(deneme123 PRIVATE ${OpenCV_LIBS})
target_include_directories(deneme123 PRIVATE ${OpenCV_INCLUDE_DIRS})
