cmake_minimum_required(VERSION 2.8)

project(opengl-ga)

add_executable(opengl-ga display.c opencl.c gautil.c generate.c jpeg.c vectimg.c gltext.c lodepng/lodepng.c)

add_definitions(-std=c99 -msse2)

FIND_LIBRARY(OPENCL_LIBRARIES OpenCL DOC "OpenCL lib for OSX")
FIND_PATH(OPENCL_INCLUDE_DIRS OpenCL/cl.h DOC "Include for OpenCL on OSX")

add_subdirectory(glfw)
include_directories(glfw/include)
include_directories(jpeg)
include_directories(lodepng)
include_directories(${OPENCL_INCLUDE_DIRS})

target_link_libraries(opengl-ga ${CMAKE_SOURCE_DIR}/jpeg/.libs/libjpeg.a glfw ${GLFW_LIBRARIES} ${OPENCL_LIBRARIES})
