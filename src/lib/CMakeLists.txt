set(lib_SRC Face.cpp Maths.cpp NormalCalculation.cpp Object.cpp Vertex.cpp SequentialNormalCalculation.cpp OpenMPNormalCalculation.cpp Vector.cpp)

add_library(common_lib STATIC ${lib_SRC})
target_include_directories(common_lib SYSTEM PUBLIC ${INCLUDE_DIR})
