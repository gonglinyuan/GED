cmake_minimum_required(VERSION 3.5)
project(GED)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O2)
add_compile_options(-march=native)

add_executable(GED main.cpp Graph.cpp Graph.h Model.cpp Model.h main.h GraphTreeSolver.cpp GraphTreeSolver.h
        TreeSolver.h TreeSolver.cpp)

#add_executable(IPsolver_test IPsolver_test.cpp IPSolver.cpp IPSolver.h Simplex.cpp Simplex.h)
#
#add_executable(Simplex_test SimplexTest.cpp)
#
#add_executable(GEDSolver_test GEDSolver_test.cpp GEDSolver.h GEDSolver.cpp Graph.h Graph.cpp
#        Simplex.h Simplex.cpp)

#add_executable(TreeSolver_test TreeSolver_test.cpp TreeSolver.cpp TreeSolver.h)
#
#add_executable(GraphTreeSolver_test GraphTreeSolver_test.cpp GraphTreeSolver.cpp GraphTreeSolver.h
#        TreeSolver.cpp TreeSolver.h Graph.cpp Graph.h)

find_package(Threads)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})