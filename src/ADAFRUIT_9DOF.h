#ifndef __ADAFRUIT_9DOF_H__
#define __ADAFRUIT_9DOF_H__

#include <stdint.h>
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
    void calcCoord();
    void setAutoRange();
    point3D accel;
    point3D mag;
    point3D gyro;
    point3D correction;
    float yaw, pitch, roll;
private:
    int file;
    sensorID_t currSensor;
};

#endif
