CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(PA2 CXX)

SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -fopenmp")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -fopenmp")
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PATH(OPENGL_INCLUDE_SUBDIR
        NAMES gl.h
        PATHS ${OPENGL_INCLUDE_DIR}
        PATH_SUFFIXES GL OpenGL Headers
        NO_DEFAULT_PATH)

FIND_PACKAGE(GLUT REQUIRED)
FIND_PATH(GLUT_INCLUDE_SUBDIR
        NAMES glut.h
        PATHS ${GLUT_INCLUDE_DIR}
        PATH_SUFFIXES GL GLUT Headers
        NO_DEFAULT_PATH)

ADD_SUBDIRECTORY(deps/vecmath)

SET(PA2_SOURCES
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/scene_parser.cpp
        src/camera_controller.cpp
        src/texture.cpp)

SET(PA2_INCLUDES
        include/camera.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/light.hpp
        include/material.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/plane.hpp
        include/ray.hpp
        include/scene_parser.hpp
        include/sphere.hpp
        include/transform.hpp
        include/triangle.hpp
        include/camera_controller.hpp
        include/curve.hpp
        include/revsurface.hpp
        include/ray_tracing.hpp
        include/path_tracing.hpp
        include/stb_image.h
        include/texture.hpp
        include/utils.hpp
        include/NEE.hpp)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-g -O3 -Wno-deprecated-declarations")

ADD_EXECUTABLE(${PROJECT_NAME} ${PA2_SOURCES} ${PA2_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include ${OPENGL_INCLUDE_SUBDIR}  ${GLUT_INCLUDE_SUBDIR})
