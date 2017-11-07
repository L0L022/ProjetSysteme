set(gui_SRC Object.cpp main.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets Qml Quick
                            3DCore 3DRender 3DInput 3DExtras 3DQuick 3DLogic 3DQuickExtras)

FILE(GLOB gui_HEADER ${INCLUDE_DIR}/gui/*.hpp)

add_executable(gui ${gui_HEADER} ${gui_SRC} ${QML_DIR}/qml.qrc)
target_link_libraries(gui common_lib)
target_link_libraries(gui Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick
                          Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DExtras Qt5::3DQuick Qt5::3DLogic Qt5::3DQuickExtras)
install(TARGETS gui DESTINATION bin RENAME ${PROJECT_NAME}-gui)
