#include "LSM303.h"
#include "L3GD20.h"
#include "ADAFRUIT_9DOF.h"
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>

int main() {
    ADAFRUIT_9DOF self;
    //self.initialize();
    while(1) {
        self.readAll();
        printf("GYRO:    X: %f   Y: %f   Z: %f\n", self.gyro.x, self.gyro.y, self.gyro.z);
        printf("ACCEL:   X: %f   Y: %f   Z: %f\n", self.accel.x, self.accel.y, self.accel.z);
        printf("MAG:     X: %f   Y: %f   Z: %f\n", self.mag.x, self.mag.y, self.mag.z);
        //Sleep for 0.25ms
		usleep(250000);
    }
}
