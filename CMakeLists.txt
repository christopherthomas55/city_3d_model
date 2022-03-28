cmake_minimum_required(VERSION 3.1...3.22)
set (CMAKE_CXX_STANDARD 11)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

project(main_open_gl)

find_package(CGAL REQUIRED OPTIONAL COMPONENTS Qt5)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
#add_library(GLAD "glad.c")

set(LIBS glfw GLU CGAL dl gmp)

file(GLOB_RECURSE SOURCE_FILES 
    src/*.c
    src/*.cpp
    glad.c)

file(GLOB_RECURSE HEADER_FILES 
    src/*.h
    src/*.hpp)

include_directories(/usr/include/ includes/)

add_executable(main_open_gl main.cpp ${HEADER_FILES} ${SOURCE_FILES})

add_executable(buildings test_building.cpp ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(main_open_gl ${LIBS})



#if(CGAL_Qt5_FOUND)
#  #link it with the required CGAL libraries
#  target_link_libraries(main_open_gl PUBLIC CGAL::CGAL_Basic_viewer)
#endif()
