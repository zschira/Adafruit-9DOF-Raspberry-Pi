#include "ADAFRUIT_9DOF.h"
#include <iostream>
using namespace std;

int main() {
    ADAFRUIT_9DOF imu;
    imu.setAutoRange();
    int counter = 0;
    float quaternion[4] = {1.0, 0.0, 0.0, 0.0};
    while(counter < 10000) {
        imu.calcCoord(quaternion);
        printf("%f   %f   %f   %f\n", quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
        //Sleep for 0.25ms
		//usleep(250000);
        counter++;
    }
}
