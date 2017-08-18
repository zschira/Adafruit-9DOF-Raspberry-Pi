#include "LSM303.h"
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

//Conversion constants
static float _lsm303Accel_MG_LSB     = 0.000969F;   // 1, 2, 4 or 12 mg per lsb
static float _lsm303Mag_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _lsm303Mag_Gauss_LSB_Z  = 980.0F;   // Varies with gain

void LSM303::initialize() {
    char filename[20];
	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);

	file = open(filename, O_RDWR);
	if (file<0) {
    	printf("Unable to open I2C bus!");
        printf("%s\n", strerror(errno));
        exit(1);
	}
    magRange = false;
    enableAccel();
    enableMag();
}

void LSM303::initUnified(int file_global) {
    file = file_global;
    magRange = false;
    enableAccel();
    enableMag();
}

///////////////////////////////////////////////////////////////////////////////
// Accelerometer Functions
///////////////////////////////////////////////////////////////////////////////

//Private Functions
void LSM303::enableAccel() {
    //Select the accelerometer
    printf("Selecting accelerometer:\n");
	if (ioctl(file, I2C_SLAVE, LSM303_ADDRESS_ACCEL) < 0) {
        printf("Error: Could not select accelerometer\n");
    }
    printf("%s\n", strerror(errno));
    // Set update rate to 100Hz
    writeReg(LSM303_REGISTER_ACCEL_CTRL_REG1_A, 0x57);
    currSensor = true;
}

void LSM303::selectAccel() {
    if (ioctl(file, I2C_SLAVE, LSM303_ADDRESS_ACCEL) < 0) {
        printf("Error: Could not select accelerometer\n");
        printf("%s\n", strerror(errno));
    }
    currSensor = true;
}

//Public Functions
void LSM303::readAccel(int16_t *accelRaw, float *accel) {
    if(!currSensor) {
        selectAccel();
    }
    uint8_t data[6], command = LSM303_REGISTER_ACCEL_OUT_X_L_A | 0x80;
    int bytes = 8;
    int result = i2c_smbus_read_i2c_block_data(file, command, bytes, data);
    if (result != bytes) {
        printf("Failed to read accel block from I2C.\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
    // Convert to 12 bit int raw acceleration values
    accelRaw[0] = (int16_t)(data[0] | (data[1] << 8)) >> 4;
    accelRaw[1] = (int16_t)(data[2] | (data[3] << 8)) >> 4;
    accelRaw[2] = (int16_t)(data[4] | (data[5] << 8)) >> 4;
    // Convert to actual float values
    accel[0] = (float)accelRaw[0] * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
    accel[1] = (float)accelRaw[1] * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
    accel[2] = (float)accelRaw[2] * _lsm303Accel_MG_LSB * SENSORS_GRAVITY_STANDARD;
}

///////////////////////////////////////////////////////////////////////////////
// Magnetometer Functions
///////////////////////////////////////////////////////////////////////////////

//Private Functions
void LSM303::enableMag() {
    //Select the magnetomoter
    printf("Selecting Magnetometer:\n");
	if (ioctl(file, I2C_SLAVE, LSM303_ADDRESS_MAG) < 0) {
        printf("Error: Could not select magnetometer\n");
    }
    printf("%s\n", strerror(errno));
    // Set rate
    writeReg(LSM303_REGISTER_MAG_MR_REG_M, 0x00);
    setMagRate(LSM303_MAGRATE_75);
    currSensor = false;
}

void LSM303::selectMag() {
    if (ioctl(file, I2C_SLAVE, LSM303_ADDRESS_MAG) < 0) {
        printf("Error: Could not select magnetometer\n");
        printf("%s\n", strerror(errno));
    }
    currSensor = false;
}

//Public Functions
void LSM303::readMag(int16_t *magRaw, float *mag) {
    if(currSensor) {
        selectMag();
    }
    uint8_t data[6], command = LSM303_REGISTER_MAG_OUT_X_H_M;
    uint8_t bytes = 6;
    int result = i2c_smbus_read_i2c_block_data(file, command, bytes, data);
    if (result != bytes) {
        printf("Failed to read mag block from I2C.\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
    // Convert to 16 bit raw int values
    magRaw[0] = (int16_t)(data[0] | ((uint16_t)data[1] << 8));
    magRaw[1] = (int16_t)(data[2] | ((uint16_t)data[3] << 8));
    magRaw[2] = (int16_t)(data[4] | ((uint16_t)data[5] << 8));

    // Check for clipping and change gain if ranging enabled
    if(magRange) {
        /* Check if the sensor is saturating or not */
        if ( (magRaw[0] >= 2040) | (magRaw[0] <= -2040) |
            (magRaw[1] >= 2040) | (magRaw[1] <= -2040) |
           (magRaw[2] >= 2040) | (magRaw[2] <= -2040) )
        {
            /* Saturating .... increase the range if we can */
            switch(magGain) {
                case LSM303_MAGGAIN_5_6:
                    setMagGain(LSM303_MAGGAIN_8_1);
                    break;
                case LSM303_MAGGAIN_4_7:
                    setMagGain(LSM303_MAGGAIN_5_6);
                    break;
                case LSM303_MAGGAIN_4_0:
                    setMagGain(LSM303_MAGGAIN_4_7);
                    break;
                case LSM303_MAGGAIN_2_5:
                    setMagGain(LSM303_MAGGAIN_4_0);
                    break;
                case LSM303_MAGGAIN_1_9:
                    setMagGain(LSM303_MAGGAIN_2_5);
                    break;
                case LSM303_MAGGAIN_1_3:
                    setMagGain(LSM303_MAGGAIN_1_9);
                    break;
                default:
                    break;
            }
        }
    }
    //Convert to floats
    mag[0] = (float)magRaw[0] * _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    mag[1] = (float)magRaw[1] * _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    mag[2] = (float)magRaw[2] * _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    return;
}

void LSM303::setMagGain(lsm303MagGain gain) {
    if(currSensor) {
        selectMag();
    }
    writeReg(LSM303_REGISTER_MAG_CRB_REG_M, (uint8_t)gain);

    magGain = gain;

    switch(gain) {
        case LSM303_MAGGAIN_1_3:
            _lsm303Mag_Gauss_LSB_XY = 1100;
            _lsm303Mag_Gauss_LSB_Z  = 980;
            break;
        case LSM303_MAGGAIN_1_9:
            _lsm303Mag_Gauss_LSB_XY = 855;
            _lsm303Mag_Gauss_LSB_Z  = 760;
            break;
        case LSM303_MAGGAIN_2_5:
            _lsm303Mag_Gauss_LSB_XY = 670;
            _lsm303Mag_Gauss_LSB_Z  = 600;
            break;
        case LSM303_MAGGAIN_4_0:
            _lsm303Mag_Gauss_LSB_XY = 450;
            _lsm303Mag_Gauss_LSB_Z  = 400;
            break;
        case LSM303_MAGGAIN_4_7:
            _lsm303Mag_Gauss_LSB_XY = 400;
            _lsm303Mag_Gauss_LSB_Z  = 355;
            break;
        case LSM303_MAGGAIN_5_6:
            _lsm303Mag_Gauss_LSB_XY = 330;
            _lsm303Mag_Gauss_LSB_Z  = 295;
            break;
        case LSM303_MAGGAIN_8_1:
            _lsm303Mag_Gauss_LSB_XY = 230;
            _lsm303Mag_Gauss_LSB_Z  = 205;
            break;
  }
}

void LSM303::setMagRate(lsm303MagRate rate) {
    if(currSensor) {
        selectMag();
    }
	uint8_t reg_m = ((uint8_t)rate & 0x07) << 2;
    writeReg(LSM303_REGISTER_MAG_CRA_REG_M, reg_m);
}


//////////////////////////////////////////////////////////////////////////////
// Shared Functions
//////////////////////////////////////////////////////////////////////////////

void LSM303::writeReg(uint8_t reg, uint8_t value) {
    int result = i2c_smbus_write_byte_data(file, reg, value);
    if (result == -1) {
        printf ("Failed to write byte to I2C Reg.\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
}
