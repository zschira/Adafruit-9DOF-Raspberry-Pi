#include "ADAFRUIT_9DOF.h"
#include <iostream>
using namespace std;

int main() {
    ADAFRUIT_9DOF imu;
    imu.setAutoRange();
    int counter = 0;
    float heading;
    while(counter < 100) {
        imu.readAll();
        cout << "GYRO: " << imu.gyro << endl;
        cout << "ACCEl: " << imu.accel << endl;
        cout << "MAG: " << imu.mag << endl;
        //Sleep for 0.25ms
		//usleep(250000);
        counter++;
    }
}
