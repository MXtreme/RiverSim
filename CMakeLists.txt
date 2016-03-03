cmake_minimum_required(VERSION 3.3)
project(RiverSim)

find_package(Threads REQUIRED)
find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)
find_package(CMinpack REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Sources/Canale.cpp
    Sources/Canale.h
    Sources/Elemento.cpp
    Sources/Elemento.h
    Sources/ElemGraph.cpp
    Sources/ElemGraph.h
    Sources/Generatore.cpp
    Sources/Generatore.h
    Sources/Graph.cpp
    Sources/Graph.h
    Sources/main.cpp
    Sources/MyGlWindow.cpp
    Sources/MyGlWindow.h
    Sources/Nodo.cpp
    Sources/Nodo.h
    Sources/Parser.h
    Sources/Pompa.cpp
    Sources/Pompa.h
    Sources/ProvaParser.cpp
    Sources/ProvaParser.h
    Sources/resource.h
    Sources/Simulazione.cpp
    Sources/Simulazione.h
    Sources/stdafx.cpp
    Sources/stdafx.h
    Sources/WindowManager.cpp
    Sources/WindowManager.h)

add_executable(RiverSim ${SOURCE_FILES})
target_link_libraries(RiverSim ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(RiverSim ${FLTK_LIBRARIES})
target_link_libraries(RiverSim ${OPENGL_LIBRARIES})
target_link_libraries(RiverSim cminpack)
target_link_libraries(RiverSim png12)