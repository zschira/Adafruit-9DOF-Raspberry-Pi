cmake_minimum_required(VERSION 2.8.9)
project(Adafruit-9DOF-Raspberry-Pi)

include_directories(src)
file(GLOB SOURCES "src/*.cpp")

add_executable(driverEx ${SOURCES})
