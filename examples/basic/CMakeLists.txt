set(CMAKE_C_STANDARD 99)

add_executable(basic_term src/main.c)
target_link_libraries(basic_term PRIVATE jadeitite_term_shared)
