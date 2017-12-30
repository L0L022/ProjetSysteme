set(benchmark_SRC main.cpp)

# 1.3 required
find_package(benchmark REQUIRED)

add_executable(bench ${benchmark_SRC})
set_target_properties(bench PROPERTIES OUTPUT_NAME benchmark)

target_link_libraries(bench common_lib)
target_link_libraries(bench benchmark::benchmark)
