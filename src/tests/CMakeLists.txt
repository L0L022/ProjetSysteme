enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Test)

add_executable(TestNormalCalculation TestNormalCalculation.cpp ${INCLUDE_DIR}/tests/TestNormalCalculation.hpp)
target_link_libraries(TestNormalCalculation common_lib)
target_link_libraries(TestNormalCalculation Qt5::Core Qt5::Test)
add_test(NAME TestNormalCalculation COMMAND TestNormalCalculation)
