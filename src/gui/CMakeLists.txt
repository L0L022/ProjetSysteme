set(gui_SRC main.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Qml)
find_package(Qt5Quick)

find_package(Qt53DCore)
find_package(Qt53DRender)
find_package(Qt53DInput)
find_package(Qt53DExtras)
find_package(Qt53DQuick)
find_package(Qt53DLogic)
find_package(Qt53DQuickExtras)

add_executable(gui ${gui_SRC} ${QML_DIR}/qml.qrc)
target_link_libraries(gui common_lib)
target_link_libraries(gui Qt5::Core Qt5::Widgets Qt5::Qml Qt5::Quick)
install(TARGETS gui DESTINATION bin RENAME ${PROJECT_NAME}-gui)
