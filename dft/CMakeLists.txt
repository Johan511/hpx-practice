cmake_minimum_required(VERSION 3.18)
project(dft CXX)
find_package(HPX REQUIRED)
add_executable(dft main.cpp dft.cpp data_IR.cpp data_Red.cpp)
target_link_libraries(dft HPX::hpx )