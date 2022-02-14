cmake_minimum_required(VERSION 3.20)
project(Clioncodes C)

set(CMAKE_C_STANDARD 11)

add_executable(fighter fighter.c)
add_executable(tmp tmp.c)
add_executable(tipFromAssistance tipFromAssistance.c)
target_link_libraries(fighter mingw32)
target_link_libraries(fighter SDL2main)
target_link_libraries(fighter SDL2)
target_link_libraries(fighter SDL2_image)
target_link_libraries(fighter SDL2_ttf)