cmake_minimum_required (VERSION 3.8)

set (MY_NAME "remote-hands")
project (${MY_NAME})

set (MY_FILES
        "main.cpp"
        "shape.h" "shape.cpp"
        "vec.h"
        "shape_group.cpp" "shape_group.h"
        "nested_shape.cpp" "nested_shape.h")

if (WIN32)
    include_directories("include")
    link_directories("lib")
endif (WIN32)

add_executable (${MY_NAME} ${MY_FILES})

if (WIN32)
    target_link_libraries (${MY_NAME} "freeglut")
    add_custom_command (TARGET ${MY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/dll/freeglut.dll" ${CMAKE_BINARY_DIR})
endif (WIN32)

if (UNIX)
    target_link_libraries (${MY_NAME} "GL" "GLU" "glut")
endif (UNIX)
