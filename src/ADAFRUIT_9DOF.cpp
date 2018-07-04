#include "ADAFRUIT_9DOF.h"
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

void ADAFRUIT_9DOF::calcCoord() {
	float cr, sr, cp, sp, magcx, magcy;
	readAll();
	correction.x = correction.y = correction.z = 0;
	roll = atan2(accel.y, accel.z);
	cr = cos(roll); sr = sin(roll);
	pitch = atan2(-accel.x, accel.y*sr + accel.z*cr);
	cp = cos(pitch); sp = sin(pitch);
	magcy = (mag.z - correction.z)*sr - (mag.y - correction.y)*cr;
	magcx = (mag.x - correction.x)*cp + (mag.y - correction.y)*sr*sp + (mag.z - correction.z)*sp*cr;
	yaw = atan2(-magcy, magcx);
}

//////////////////////////////////////////////////////////////////////////////
// Functions to change sensor Parameters
//////////////////////////////////////////////////////////////////////////////

void ADAFRUIT_9DOF::setAutoRange() {
    gyroRange = !(gyroRange);
    magRange = !(magRange);
}
