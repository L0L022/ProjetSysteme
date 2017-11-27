set(lib_SRC Face.cpp Maths.cpp NormalCalculation.cpp Object.cpp Vertex.cpp SequentialNormalCalculation.cpp OpenMPNormalCalculation.cpp PThreadNormalCalculation.cpp Vector.cpp)

add_library(common_lib STATIC ${lib_SRC})
target_include_directories(common_lib SYSTEM PUBLIC ${INCLUDE_DIR})

find_package(Threads REQUIRED)
IF(THREADS_FOUND)
  target_link_libraries (common_lib ${CMAKE_THREAD_LIBS_INIT})
ENDIF()
