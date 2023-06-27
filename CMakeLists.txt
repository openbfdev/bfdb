cmake_minimum_required(VERSION 3.25)
project(bfdb LANGUAGES C CXX)

add_definitions(
    " -Wall"
    " -Wextra"
    " -Wno-override-init"
    " -Wno-unused-parameter"
    " -Wno-sign-compare"
    " -Wno-pointer-sign"
    " -Wno-null-pointer-arithmetic"
)

set(CMAKE_CXX_STANDARD 14)

set(BFDEV_SOURCE_PATH ${PROJECT_SOURCE_DIR}/src)


file(GLOB HEADERS
    ${BFDEV_SOURCE_PATH}/*.hpp
)

file(GLOB SRCS
    ${HEADERS}
    ${BFDEV_SOURCE_PATH}/*.cpp
)

add_subdirectory(src/bfdev)
include_directories(${PROJECT_SOURCE_DIR}/src/bfdev/include)
include_directories(${PROJECT_BINARY_DIR}/src/bfdev/generated)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(bfdb_shared SHARED ${SRCS})
add_library(bfdb_static STATIC ${SRCS})
add_library(bfdb ALIAS bfdb_static)

target_link_libraries(bfdb_static PRIVATE bfdev)
target_link_libraries(bfdb_shared PRIVATE bfdev)


add_subdirectory(demo)

install(TARGETS
    bfdb_static 
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)