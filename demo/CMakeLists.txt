

add_executable(db main.cpp)
target_link_libraries(db bfdb)


add_executable(mtable mtable.cpp)
target_link_libraries(mtable bfdb)