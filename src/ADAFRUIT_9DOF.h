#ifndef __ADAFRUIT_9DOF_H__
#define __ADAFRUIT_9DOF_H__

#include <stdint.h>
#include <Eigen/Dense>
#include "LSM303.h"
#include "L3GD20.h"

using namespace Eigen;

typedef enum {
    ACCELEROMETER     =  0,
    MAGNETOMETER      =  1,
    GYROSCOPE         =  2
}sensorID_t;

class ADAFRUIT_9DOF : public LSM303, public L3GD20 {
public:
    ADAFRUIT_9DOF();
    ~ADAFRUIT_9DOF();
    void readAll();
    void calcCoord();
    void setAutoRange();
    Vector3f accel;
    Vector3f mag;
    Vector3f gyro;
    Vector3f correction;
    float yaw, pitch, roll;
private:
    int file;
    sensorID_t currSensor;
};

#endif
