cmake_minimum_required(VERSION 3.15)
project(mySloc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib/OpenXLSX-master/OpenXLSX)
add_executable(mySloc main.cpp lineAnalyze.cpp)

# Link stdc++fs for filesystem support
target_link_libraries(mySloc stdc++fs)
target_link_libraries(mySloc OpenXLSX::OpenXLSX)
target_include_directories(mySloc PRIVATE lib/OpenXLSX-master/OpenXLSX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_definitions(-DUNICODE -D_UNICODE)