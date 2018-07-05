#include "ADAFRUIT_9DOF.h"
#include "MadwickAHRS.h"
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cmath>

ADAFRUIT_9DOF::ADAFRUIT_9DOF() {
    char filename[20];
	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);

	file = open(filename, O_RDWR);
	if (file<0) {
    	printf("Unable to open I2C bus!");
        printf("%s\n", strerror(errno));
        exit(1);
	}
	initL3GD20(file);
	initLSM303(file);
    currSensor = MAGNETOMETER;
};

ADAFRUIT_9DOF::~ADAFRUIT_9DOF() {close(file);};

void ADAFRUIT_9DOF::readAll() {
    readAccel(&accel.x);
    readMag(&mag.x);
    readGyro(&gyro.x);
}

void ADAFRUIT_9DOF::calcCoord(float quaternion[4]) {
	MadgwickAHRSupdate(gyro.x, gyro.y, gyro.z,accel.x, accel.y, accel.z, mag.x, mag.y, mag,z, quaternion);
}

//////////////////////////////////////////////////////////////////////////////
// Functions to change sensor Parameters
//////////////////////////////////////////////////////////////////////////////

void ADAFRUIT_9DOF::setAutoRange() {
    gyroRange = !(gyroRange);
    magRange = !(magRange);
}
