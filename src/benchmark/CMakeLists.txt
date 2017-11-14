set(benchmark_SRC main.cpp)

find_package(benchmark REQUIRED)
add_executable(bench ${benchmark_SRC})
target_link_libraries(bench common_lib)
target_link_libraries(bench benchmark::benchmark)
