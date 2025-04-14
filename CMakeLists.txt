cmake_minimum_required(VERSION 3.10)
project(picoscope)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "/opt/picoscope/include")
target_link_directories(${PROJECT_NAME} PRIVATE "/opt/picoscope/lib")
target_link_libraries(${PROJECT_NAME} PRIVATE ps4000a pugixml)