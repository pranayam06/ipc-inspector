cmake_minimum_required(VERSION 3.15)

project(ipc_inspector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)


#add_executable(producer src/producer.cpp src/ring_buffer.cpp)
#add_executable(consumer src/consumer.cpp src/ring_buffer.cpp)
#add_executable(inspector src/inspector.cpp src/ring_buffer.cpp)

add_executable(producer src/producer.cpp src/ring_buffer_mutex.cpp)
add_executable(consumer src/consumer.cpp src/ring_buffer_mutex.cpp)
add_executable(inspector src/inspector_mtex.cpp src/ring_buffer_mutex.cpp)
add_executable(clearshm src/clean.cpp)


# target_link_libraries()
set(CMAKE_BUILD_TYPE Debug)