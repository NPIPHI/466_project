cmake_minimum_required(VERSION 3.20)
project(algo_project)

set(CMAKE_CXX_STANDARD 20)

add_executable(algo_project main.cpp exact.cpp exact.h algo.h Synthetic.cpp Synthetic.h mem.h bound_exact.cpp bound_exact.h star.cpp star.h tree.cpp tree.h)
