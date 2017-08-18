#include "ADAFRUIT_9DOF.h"
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

ADAFRUIT_9DOF::ADAFRUIT_9DOF() {
    sensor1 = new L3GD20;
    sensor2 = new LSM303;
    char filename[20];
	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);

	file = open(filename, O_RDWR);
	if (file<0) {
    	printf("Unable to open I2C bus!");
        printf("%s\n", strerror(errno));
        exit(1);
	}
    sensor1->initUnified(file);
    sensor2->initUnified(file);
    currSensor = MAGNETOMETER;
};

ADAFRUIT_9DOF::~ADAFRUIT_9DOF() {
    delete sensor1;
    delete sensor2;
};

void ADAFRUIT_9DOF::readAccel() {
    if(currSensor) {
        sensor2->selectAccel();
    }
    int16_t accelRaw[3];
    float accelActual[3];
    sensor2->readAccel(accelRaw, accelActual);
    accel.x = accelActual[0];
    accel.y = accelActual[1];
    accel.z = accelActual[2];
    currSensor = ACCELEROMETER;
}
void ADAFRUIT_9DOF::readMag() {
    if(currSensor != 1) {
        sensor2->selectMag();
    }
    int16_t magRaw[3];
    float magActual[3];
    sensor2->readMag(magRaw, magActual);
    mag.x = magActual[0];
    mag.y = magActual[1];
    mag.z = magActual[2];
    currSensor = MAGNETOMETER;
}
void ADAFRUIT_9DOF::readGyro() {
    if(currSensor) {
        sensor1->selectGyro();
    }
    int16_t gyroRaw[3];
    float gyroActual[3];
    sensor1->readGyro(gyroRaw, gyroActual);
    gyro.x = gyroActual[0];
    gyro.y = gyroActual[1];
    gyro.z = gyroActual[2];
    currSensor = GYROSCOPE;
}
void ADAFRUIT_9DOF::readAll() {
    readAccel();
    readMag();
    readGyro();
}
