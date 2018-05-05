cmake_minimum_required(VERSION 3.5)
project(GED)

set(CMAKE_CXX_STANDARD 11)

add_executable(GED main.cpp Graph.cpp Graph.h Model.cpp Model.h /home/gonglinyuan/gurobi800/linux64/include/gurobi_c++.h main.h)
target_include_directories(GED PRIVATE /home/gonglinyuan/gurobi800/linux64/include)
target_link_libraries(GED /home/gonglinyuan/gurobi800/linux64/lib/libgurobi_g++5.2.a /home/gonglinyuan/gurobi800/linux64/lib/libgurobi80.so)