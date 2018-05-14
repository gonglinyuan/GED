cmake_minimum_required(VERSION 3.5)
project(GED)

set(CMAKE_CXX_STANDARD 11)

add_executable(GED main.cpp Graph.cpp Graph.h Model.cpp Model.h main.h IPSolver.h
        IPSolver.cpp Simplex.cpp Simplex.h GEDSolver.cpp GEDSolver.h)
target_include_directories(GED PRIVATE)

add_executable(IPsolver_test IPsolver_test.cpp IPSolver.cpp IPSolver.h Simplex.cpp Simplex.h)

add_executable(Simplex_test SimplexTest.cpp)

add_executable(GEDSolver_test GEDSolver_test.cpp GEDSolver.h GEDSolver.cpp Graph.h Graph.cpp
        Simplex.h Simplex.cpp)