cmake_minimum_required(VERSION 3.0)

set(NAME manipulator_simulation)

add_executable(${NAME} main.cpp std_utils.h arms.h arms_animations.h forward_kinematics.h forward_kinematics_animations.h inverse_kinematics.h inverse_kinematics_animations.h)

target_include_directories(${NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/extern/tcp-cpp/include"
        "${PROJECT_SOURCE_DIR}/extern/serial-cpp/include"
        "${PROJECT_SOURCE_DIR}/extern/mathematics-c/include")

target_link_libraries(${NAME} PRIVATE GL GLU glut shape_groups tcp_cpp serial_cpp mathematics_c)

install(TARGETS ${NAME} DESTINATION "${CMAKE_INSTALL_BINDIR}")