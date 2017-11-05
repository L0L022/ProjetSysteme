cmake_minimum_required(VERSION 3.0)

project(ProjetSysteme)

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(QML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qml)
add_subdirectory(src)

install(FILES ${PROJECT_NAME}.desktop DESTINATION share/applications)
