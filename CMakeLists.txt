cmake_minimum_required(VERSION 3.3.0)
project(Adafruit-9DOF-Raspberry-Pi)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
if(RENDER)
	add_definitions(-DRENDER)
	add_library(STB_IMAGE "opengl/src/stb_image.cpp")
	set(LIBS ${LIBS} STB_IMAGE)
	include_directories(opengl/includes)

	add_library(GLAD "opengl/src/glad.c")
	target_link_libraries(GLAD dl)
	target_compile_options(GLAD PUBLIC -ldl)
	set(LIBS ${LIBS} GLAD)

	include_directories ("${PROJECT_SOURCE_DIR}/opengl/src")
	add_subdirectory(opengl)
endif(RENDER)


add_executable(imu src/main.cpp src/L3GD20.cpp src/LSM303.cpp src/ADAFRUIT_9DOF.cpp)
target_compile_options(imu PUBLIC "$<$<CONFIG:DEBUG>:-ggdb>")
target_compile_options(imu PUBLIC "$<$<CONFIG:RELEASE>:-O3>")