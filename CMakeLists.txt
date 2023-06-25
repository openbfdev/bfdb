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

add_executable(bfdb ${SRCS})

target_link_libraries(${PROJECT_NAME}  PRIVATE bfdev)

install(TARGETS
    ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)