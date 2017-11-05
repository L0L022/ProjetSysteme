set(cli_SRC main.cpp)

add_executable(cli ${cli_SRC})
target_link_libraries(cli common_lib)
install(TARGETS cli DESTINATION bin RENAME ${PROJECT_NAME}-cli)
