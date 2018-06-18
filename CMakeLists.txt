cmake_minimum_required(VERSION 2.8.9)
project(Adafruit-9DOF-Raspberry-Pi)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(imu src/main.cpp src/L3GD20.cpp src/LSM303.cpp ADAFRUIT_9DOF.cpp)
