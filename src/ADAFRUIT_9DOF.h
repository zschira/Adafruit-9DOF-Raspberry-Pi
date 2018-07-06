#ifndef __ADAFRUIT_9DOF_H__
#define __ADAFRUIT_9DOF_H__

#include <stdint.h>
#include <chrono>
#include "LSM303.h"
#include "L3GD20.h"

typedef enum {
    ACCELEROMETER     =  0,
    MAGNETOMETER      =  1,
    GYROSCOPE         =  2
}sensorID_t;

struct point3D {
    float x;
    float y;
    float z;
};

class ADAFRUIT_9DOF : public LSM303, public L3GD20 {
public:
    ADAFRUIT_9DOF();
    ~ADAFRUIT_9DOF();
    void readAll();
    void calcCoord(float quaternion[4]);
    void setAutoRange();
    point3D accel;
    point3D mag;
    point3D gyro;
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    float rate;
private:
    int file;
    sensorID_t currSensor;
};

#endif