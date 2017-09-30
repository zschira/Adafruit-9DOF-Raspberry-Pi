cmake_minimum_required(VERSION 2.8.9)
project(Adafruit-9DOF-Raspberry-Pi)

include_directories(inc)
file(GLOB SOURCES "src/*.cpp")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../rpi_exe")

add_executable(driverEx ${SOURCES})
