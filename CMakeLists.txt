cmake_minimum_required(VERSION 3.1)

project(ProjetSysteme)

set (CMAKE_CXX_STANDARD 14)

option(COMPILE_CLI "Compile cli application." ON)
option(COMPILE_GUI "Compile gui application." ON)
option(COMPILE_BENCHMARK "Compile benchmark test." ON)
option(COMPILE_TESTS "Compile tests." ON)

set(benchmark_DIR "/home/e16006130/Téléchargements/bb/install/usr/local/lib/cmake/benchmark")
list(APPEND CMAKE_MODULE_PATH "/home/e16006130/Téléchargements/bb/install/usr/local/lib/cmake")

find_package(OpenMP REQUIRED)
IF(OPENMP_FOUND)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF()

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(QML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qml)
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
add_subdirectory(src)
add_subdirectory(assets)

install(FILES ${PROJECT_NAME}.desktop DESTINATION share/applications)
