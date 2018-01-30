cmake_minimum_required(VERSION 2.8)
project(CoinsOpenCV)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(CoinsOpenCV ${SOURCE_FILES})
target_link_libraries( CoinsOpenCV ${OpenCV_LIBS} )