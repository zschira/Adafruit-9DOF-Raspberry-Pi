cmake_minimum_required(VERSION 3.3.0)
project(Adafruit-9DOF-Raspberry-Pi)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
find_package (Eigen3 REQUIRED)
include_directories(${Eigen3_INCLUDE_DIRS})
include_directories(/usr/include/eigen3)

add_executable(imu src/main.cpp src/L3GD20.cpp src/LSM303.cpp src/ADAFRUIT_9DOF.cpp)