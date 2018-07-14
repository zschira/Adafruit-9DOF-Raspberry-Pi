# Adafruit_9DOF_IMU-RPI-Driver

## Description
Adafruit 9dof C++ driver for Raspberry pi support. Contains individual drivers for both the LSM303 and L3GD20 sensors, and a unified driver for the entire IMU.

## Installation
The CMakeLists.txt file shows how to include the driver in a project. This compiles main.cpp which shows basic usage of the driver. You should be able to easily include this in any cmake project.

## Usage
The ADAFRUIT_9DOF class inherits from the LSM303 and L3GD20 classes. I am using the MadgwickAHRS algorithm to calculate the orientation of the IMU. To enable communication the IMU simply create an instance of the ADAFRUIT_9DOF class. From here you can read from each individual sensor using the `readAccell()`, `readMag()` and `readGyro()` functions (these all take a pointer to an array of 3 floats floats and will write the processed 3D vector read from sensor to this array). To read all sensors at one time use the `readAll()` function. This will store the results in the `accel`, `mag` and `gyro` structures in the class. To calculate the orientation use the `calcCoord()` function. This function takes a pointer to an array of 4 floats and writes the 4D vector quaternion to the array. This is demonstrated in main.cpp.
