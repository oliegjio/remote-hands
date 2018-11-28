cmake_minimum_required (VERSION 3.8)

set (MY_NAME "remote-hands")
project (${MY_NAME})

set (MY_FILES
        "main.cpp"
        "vector.h"
        "matrix.h"
        "mathematics.cpp" "mathematics.h"
        "shape.h" "shape.cpp"
        "nested_shape.cpp" "nested_shape.h"
        "shape_group.cpp" "shape_group.h"
        "nested_group.cpp" "nested_group.h"
        "arms.h"
        "arms_animations.h"
        "forward_kinematics.h"
        "inverse_kinematics.h"
        "inverse_kinematics_animations.h"
        "forward_kinematics_animations.h"
        "server.cpp" "server.h"
        "string_utils.h"
        "quaternion.cpp" "quaternion.h")

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
