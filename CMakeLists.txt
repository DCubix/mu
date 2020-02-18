cmake_minimum_required(VERSION 3.11)
project(mu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC "src/*.h" "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})