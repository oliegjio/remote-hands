cmake_minimum_required(VERSION 3.0)

set(NAME shape_groups)

add_library(${NAME} STATIC SHARED nested_group.cpp nested_shape.cpp shape.cpp shape_group.cpp)

target_include_directories(${NAME}
        PUBLIC "${PROJECT_SOURCE_DIR}/include/${NAME}"
        PRIVATE "${PROJECT_SOURCE_DIR}/src/${NAME}")

target_link_libraries(${NAME} PUBLIC GL GLU glut mathematics_c)

install(TARGETS ${NAME} DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/${NAME}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")