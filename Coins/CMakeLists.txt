cmake_minimum_required(VERSION 2.8)
project(Coins)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(Coins ${SOURCE_FILES})
target_link_libraries( Coins ${OpenCV_LIBS} )
