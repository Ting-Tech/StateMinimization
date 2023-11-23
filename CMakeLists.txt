cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)

project(StateMinimization)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(SRC_LIST
    ${PROJECT_SOURCE_DIR}/src/KissHandler.cc
    ${PROJECT_SOURCE_DIR}/src/Minimization.cc
    ${PROJECT_SOURCE_DIR}/src/StateMinimization.cc
    ${PROJECT_SOURCE_DIR}/src/B11130225.cc)

add_executable(${PROJECT_NAME}.out ${SRC_LIST})