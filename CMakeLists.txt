cmake_minimum_required(VERSION 3.0)

project(ProjetSysteme)

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

install(FILES ${PROJECT_NAME}.desktop DESTINATION share/applications)
