cmake_minimum_required(VERSION 3.0)

project(polyI)

set(CMAKE_BUILD_TYPE Debue)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
    "~/SFML-2.4.2/include"
)

link_directories(
    "~/SFML-2.4.2/lib"
)

add_executable(
	polyI
	main.cpp
)

target_link_libraries(
    polyI
    sfml-system-d
    sfml-window-d
    sfml-graphics-d
)
