cmake_minimum_required(VERSION 3.20)
project(Picture C)

set(CMAKE_C_STANDARD 11)

add_executable(Snake Snake.c)
add_executable(tmp tmp.c)
target_link_libraries(Snake mingw32)
target_link_libraries(Snake SDL2main)
target_link_libraries(Snake SDL2)
target_link_libraries(Snake SDL2_image)
target_link_libraries(Snake SDL2_ttf)