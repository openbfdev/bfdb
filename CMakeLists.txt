cmake_minimum_required(VERSION 3.25)
project(bfdb LANGUAGES C CXX)

add_definitions(
    " -std=c++14"
    " -Wall"
    " -Wextra"
    " -Wno-override-init"
    " -Wno-unused-parameter"
    " -Wno-sign-compare"
    " -Wno-pointer-sign"
    " -Wno-null-pointer-arithmetic"
)

set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)


file(GLOB HEADERS
    ${BFDEV_SOURCE_PATH}/*.hpp
)

file(GLOB SRCS
    ${HEADERS}
    ${BFDEV_SOURCE_PATH}/*.cpp
)

add_executable(bfdb ${SRCS})