set(benchmark_SRC main.cpp)

add_executable(benchmark ${benchmark_SRC})
target_link_libraries(benchmark common_lib)
