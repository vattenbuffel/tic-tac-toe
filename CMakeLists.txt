cmake_minimum_required(VERSION 3.12)
project(Test VERSION 1.0.0)



add_executable(main main.c tic_tac_toe.c assert_.c min_max.c)

# Add math.h
target_link_libraries(main -lm) 








