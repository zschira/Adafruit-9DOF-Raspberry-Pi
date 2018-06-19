#include "ADAFRUIT_9DOF.h"
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

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

ADAFRUIT_9DOF::~ADAFRUIT_9DOF() { };

void ADAFRUIT_9DOF::readAll() {
    accel = readAccel();
    mag = readMag();
    gyro = readGyro();
}

//////////////////////////////////////////////////////////////////////////////
// Functions to change sensor Parameters
//////////////////////////////////////////////////////////////////////////////

void ADAFRUIT_9DOF::setAutoRange() {
    gyroRange = !(gyroRange);
    magRange = !(magRange);
}
