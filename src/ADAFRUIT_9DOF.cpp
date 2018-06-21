#include "ADAFRUIT_9DOF.h"
#include <linux/i2c-dev.h>
#include <fcntl.h>
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

ADAFRUIT_9DOF::~ADAFRUIT_9DOF() {close(file)};

void ADAFRUIT_9DOF::readAll() {
    accel = readAccel();
    mag = readMag();
    gyro = readGyro();
}

void ADAFRUIT_9DOF::calcCoord() {
	float cr, sr, cp, sp, magcx, magcy;
	correction = Vector3f::Zero();
	roll = atan2(accel(1), accel(2));
	cr = cos(roll); sr = sin(roll);
	pitch = atan2(-accel(0), accel(1)*sr + accel(2)*cr);
	cp = cos(pitch); sp = sin(pitch);
	magcy = (mag(2) - correction(2))*sr - (mag(1) - correction(1))*cr;
	magcx = (mag(0) - correction(0))*cp + (mag(1) - correction(1))*sr*sp + (mag(2) - correction(2))*sp*cr;
	yaw = atan2(-magcy, magcx);
}

//////////////////////////////////////////////////////////////////////////////
// Functions to change sensor Parameters
//////////////////////////////////////////////////////////////////////////////

void ADAFRUIT_9DOF::setAutoRange() {
    gyroRange = !(gyroRange);
    magRange = !(magRange);
}
