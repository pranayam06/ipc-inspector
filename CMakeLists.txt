cmake_minimum_required(VERSION 3.15)

project(ipc_inspector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)


add_executable(producer src/ipc/producer.cpp src/ring_buffer.cpp)
add_executable(consumer src/ipc/consumer.cpp src/ring_buffer.cpp)
add_executable(inspector src/inspectors/inspector.cpp src/ring_buffer.cpp)
add_executable(inspector-dash src/inspectors/inspector_dash.cpp src/ring_buffer.cpp)


add_executable(producer_mtex src/ipc/producer.cpp src/ring_buffer_mutex.cpp)
target_compile_definitions(producer_mtex PRIVATE USE_MUTEX)
add_executable(consumer_mtex src/ipc/consumer.cpp src/ring_buffer_mutex.cpp)
target_compile_definitions(consumer_mtex PRIVATE USE_MUTEX)
add_executable(inspector_mtex src/inspectors/inspector_mtex.cpp src/ring_buffer_mutex.cpp)
target_compile_definitions(inspector_mtex PRIVATE USE_MUTEX)


add_executable(clearshm src/clean.cpp)

# target_link_libraries()
set(CMAKE_BUILD_TYPE Debug)