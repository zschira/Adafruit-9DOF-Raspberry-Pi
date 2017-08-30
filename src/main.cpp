#include "LSM303.h"
#include "L3GD20.h"
#include "ADAFRUIT_9DOF.h"
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdio.h>

int main() {
    ADAFRUIT_9DOF self;
    self.setAutoRange();
    int counter = 0;
    float heading;
    while(counter < 100) {
        self.readAll();
        printf("GYRO:    X: %f   Y: %f   Z: %f\n", self.gyro.x, self.gyro.y, self.gyro.z);
        printf("ACCEL:   X: %f   Y: %f   Z: %f\n", self.accel.x, self.accel.y, self.accel.z);
        heading = 180*atan2(self.mag.y,self.mag.x)/M_PI;
        if(heading<0)
            heading += 360;
        printf("%f\n", heading);
        //Sleep for 0.25ms
		usleep(250000);
        counter++;
    }
}
