# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.
cmake_minimum_required(VERSION 2.6)
find_package(Rock)
rock_init(serialization 0.1)

# required by orocos
add_definitions(-DOROCOS_TARGET=gnulinux)

rock_standard_layout()
