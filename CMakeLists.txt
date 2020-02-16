cmake_minimum_required(VERSION 3.11)
project(mu)

file(GLOB SRC "src/*.h" "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})