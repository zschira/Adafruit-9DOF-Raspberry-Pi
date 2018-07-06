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
        imu.calcCoord();
        cout << "Roll: " << imu.roll << endl;
        cout << "Pitch: " << imu.pitch << endl;
        cout << "Yaw: " << imu.yaw << endl;
        //Sleep for 0.25ms
		//usleep(250000);
        counter++;
    }
}
