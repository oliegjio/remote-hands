cmake_minimum_required(VERSION 3.8)

set(MY_NAME "remote-hands")
project(${MY_NAME})

set(MY_FILES
	main.cpp
	scene_object.h
	shape.h shape.cpp
	vertex.h vertex.cpp
)

if (WIN32)
    include_directories("include")
    link_directories("lib")
endif (WIN32)

add_executable(${MY_NAME} ${MY_FILES})

if (WIN32)
    target_link_libraries(${MY_NAME} "freeglut")
endif (WIN32)

if (UNIX)
    target_link_libraries(${MY_NAME} "GL" "GLU" "glut")
endif (UNIX)
