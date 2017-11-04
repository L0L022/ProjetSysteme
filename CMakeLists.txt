cmake_minimum_required(VERSION 3.0)

project(ProjetSysteme)

include_directories(include)

set(SRC src/Face.cpp src/Object.cpp src/Point.cpp src/Vector.cpp src/Maths.cpp src/NormalCalculation.cpp src/SequentialNormalCalculation.cpp)

add_executable(main ${SRC} src/main.cpp)
