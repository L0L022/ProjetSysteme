add_subdirectory(lib)

if (COMPILE_CLI)
  add_subdirectory(cli)
endif()

if (COMPILE_GUI)
  add_subdirectory(gui)
endif()

if (COMPILE_BENCHMARK)
  add_subdirectory(benchmark)
endif()
