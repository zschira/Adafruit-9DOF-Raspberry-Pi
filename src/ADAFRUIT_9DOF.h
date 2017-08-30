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

struct accelData {
    float x;
    float y;
    float z;
};

struct magData {
    float x;
    float y;
    float z;
};

struct gyroData {
    float x;
    float y;
    float z;
};

class ADAFRUIT_9DOF {
public:
    ADAFRUIT_9DOF();
    ~ADAFRUIT_9DOF();
    void readAccel();
    void readMag();
    void readGyro();
    void readAll();
    void setMagGain(lsm303MagGain gain);
    void setMagRate(lsm303MagRate rate);
    void setAutoRange();
    accelData accel;
    magData mag;
    gyroData gyro;
private:
    int file;
    L3GD20 *sensor1;
    LSM303 *sensor2;
    sensorID_t currSensor;
};

#endif
