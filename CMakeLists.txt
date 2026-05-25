cmake_minimum_required(VERSION 3.15)

project(ipc_inspector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)


add_executable(producer src/producer.cpp)
add_executable(consumer src/consumer.cpp)
add_executable(inspector src/inspector.cpp)

# target_link_libraries()