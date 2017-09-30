#include "L3GD20.h"
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void L3GD20::initialize() {
    char filename[20];
	//Open the i2c bus
	sprintf(filename, "/dev/i2c-%d", 1);

	file = open(filename, O_RDWR);
	if (file<0) {
    	printf("Unable to open I2C bus!");
        printf("%s\n", strerror(errno));
        exit(1);
	}
    range = GYRO_RANGE_250DPS;
    enableGyro();
}

void L3GD20::initUnified(int file_global) {
    file = file_global;
    range = GYRO_RANGE_250DPS;
    enableGyro();
}

// Private Functions
void L3GD20::enableGyro() {
    printf("Selecting gyroscope:\n");
	if (ioctl(file, I2C_SLAVE, L3GD20_ADDRESS) < 0) {
        printf("Error: Could not select gyroscope\n");
    }
    printf("%s\n", strerror(errno));
    /* Set CTRL_REG1 (0x20)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
   7-6  DR1/0     Output data rate                                   00
   5-4  BW1/0     Bandwidth selection                                00
     3  PD        0 = Power-down mode, 1 = normal/sleep mode          0
     2  ZEN       Z-axis enable (0 = disabled, 1 = enabled)           1
     1  YEN       Y-axis enable (0 = disabled, 1 = enabled)           1
     0  XEN       X-axis enable (0 = disabled, 1 = enabled)           1 */
    // Reset then switch to normal mode/enable all channels
    writeReg(GYRO_REGISTER_CTRL_REG1, 0x00);
    writeReg(GYRO_REGISTER_CTRL_REG1, 0x0F);

    /* Set CTRL_REG2 (0x21)
  ====================================================================
  BIT  Symbol    Description                                   Default
  ---  ------    --------------------------------------------- -------
  5-4  HPM1/0    High-pass filter mode selection                    00
  3-0  HPCF3..0  High-pass filter cutoff frequency selection      0000 */

 /* Nothing to do ... keep default values */
 /* ------------------------------------------------------------------ */

 /* Set CTRL_REG3 (0x22)
  ====================================================================
  BIT  Symbol    Description                                   Default
  ---  ------    --------------------------------------------- -------
    7  I1_Int1   Interrupt enable on INT1 (0=disable,1=enable)       0
    6  I1_Boot   Boot status on INT1 (0=disable,1=enable)            0
    5  H-Lactive Interrupt active config on INT1 (0=high,1=low)      0
    4  PP_OD     Push-Pull/Open-Drain (0=PP, 1=OD)                   0
    3  I2_DRDY   Data ready on DRDY/INT2 (0=disable,1=enable)        0
    2  I2_WTM    FIFO wtrmrk int on DRDY/INT2 (0=dsbl,1=enbl)        0
    1  I2_ORun   FIFO overrun int on DRDY/INT2 (0=dsbl,1=enbl)       0
    0  I2_Empty  FIFI empty int on DRDY/INT2 (0=dsbl,1=enbl)         0 */

 /* Nothing to do ... keep default values */
 /* ------------------------------------------------------------------ */

 /* Set CTRL_REG4 (0x23)
  ====================================================================
  BIT  Symbol    Description                                   Default
  ---  ------    --------------------------------------------- -------
    7  BDU       Block Data Update (0=continuous, 1=LSB/MSB)         0
    6  BLE       Big/Little-Endian (0=Data LSB, 1=Data MSB)          0
  5-4  FS1/0     Full scale selection                               00
                                 00 = 250 dps
                                 01 = 500 dps
                                 10 = 2000 dps
                                 11 = 2000 dps
    0  SIM       SPI Mode (0=4-wire, 1=3-wire)                       0 */
 /* ------------------------------------------------------------------ */

 /* Adjust resolution if requested */
 switch(range)
 {
   case GYRO_RANGE_250DPS:
     writeReg(GYRO_REGISTER_CTRL_REG4, 0x00);
     break;
   case GYRO_RANGE_500DPS:
     writeReg(GYRO_REGISTER_CTRL_REG4, 0x10);
     break;
   case GYRO_RANGE_2000DPS:
     writeReg(GYRO_REGISTER_CTRL_REG4, 0x20);
     break;
 }
 /* Set CTRL_REG5 (0x24)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  BOOT      Reboot memory content (0=normal, 1=reboot)          0
     6  FIFO_EN   FIFO enable (0=FIFO disable, 1=enable)              0
     4  HPen      High-pass filter enable (0=disable,1=enable)        0
   3-2  INT1_SEL  INT1 Selection config                              00
   1-0  OUT_SEL   Out selection config                               00 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */
}


void L3GD20::writeReg(uint8_t reg, uint8_t value) {
    int result = i2c_smbus_write_byte_data(file, reg, value);
    if (result == -1) {
        printf ("Failed to write byte to I2C Reg.\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Public functions
///////////////////////////////////////////////////////////////////////////////
void L3GD20::readGyro(int16_t *gyroRaw, float *gyro) {
    uint8_t data[6], command = GYRO_REGISTER_OUT_X_L;
    uint8_t bytes = 6;
    int result = i2c_smbus_read_i2c_block_data(file, command, bytes, data);
    if (result != bytes) {
        printf("Failed to read gyro block from I2C.\n");
        printf("%s\n", strerror(errno));
        exit(1);
    }
    // Convert to 16 bit raw int values
    gyroRaw[0] = (int16_t)(data[0] | ((uint16_t)data[1] << 8));
    gyroRaw[1] = (int16_t)(data[2] | ((uint16_t)data[3] << 8));
    gyroRaw[2] = (int16_t)(data[4] | ((uint16_t)data[5] << 8));

    if(gyroRange) {
        // Check if the sensor is saturating or not
        if ( (gyroRaw[0] >= 32760) | (gyroRaw[0] <= -32760) |
           (gyroRaw[1] >= 32760) | (gyroRaw[1] <= -32760) |
           (gyroRaw[2] >= 32760) | (gyroRaw[2] <= -32760) )
        {
            // Saturating .... increase the range if we can
            switch(range) {
                case GYRO_RANGE_500DPS:
                    // Push the range up to 2000dps
                    range = GYRO_RANGE_2000DPS;
                    writeReg(GYRO_REGISTER_CTRL_REG1, 0x00);
                    writeReg(GYRO_REGISTER_CTRL_REG1, 0x0F);
                    writeReg(GYRO_REGISTER_CTRL_REG4, 0x20);
                    writeReg(GYRO_REGISTER_CTRL_REG5, 0x80);
                    break;
                case GYRO_RANGE_250DPS:
                    // Push the range up to 500dps
                    range = GYRO_RANGE_500DPS;
                    writeReg(GYRO_REGISTER_CTRL_REG1, 0x00);
                    writeReg(GYRO_REGISTER_CTRL_REG1, 0x0F);
                    writeReg(GYRO_REGISTER_CTRL_REG4, 0x10);
                    writeReg(GYRO_REGISTER_CTRL_REG5, 0x80);
                    break;
                default:
                    return;
                    break;
            }
        }
    }

    /* Compensate values depending on the resolution */
    switch(range) {
        case GYRO_RANGE_250DPS:
            gyro[0] = (float)gyroRaw[0] * GYRO_SENSITIVITY_250DPS;
            gyro[1] = (float)gyroRaw[1] * GYRO_SENSITIVITY_250DPS;
            gyro[2] = (float)gyroRaw[2] * GYRO_SENSITIVITY_250DPS;
            break;
        case GYRO_RANGE_500DPS:
            gyro[0] = (float)gyroRaw[0] * GYRO_SENSITIVITY_500DPS;
            gyro[1] = (float)gyroRaw[1] * GYRO_SENSITIVITY_500DPS;
            gyro[2] = (float)gyroRaw[2] * GYRO_SENSITIVITY_500DPS;
            break;
        case GYRO_RANGE_2000DPS:
            gyro[0] = (float)gyroRaw[0] * GYRO_SENSITIVITY_2000DPS;
            gyro[1] = (float)gyroRaw[1] * GYRO_SENSITIVITY_2000DPS;
            gyro[2] = (float)gyroRaw[2] * GYRO_SENSITIVITY_2000DPS;
            break;
    }

    /* Convert values to rad/s */
    gyro[0] *= SENSORS_DPS_TO_RADS;
    gyro[1] *= SENSORS_DPS_TO_RADS;
    gyro[2] *= SENSORS_DPS_TO_RADS;
}

void L3GD20::selectGyro() {
	if (ioctl(file, I2C_SLAVE, L3GD20_ADDRESS) < 0) {
        printf("Error: Could not select gyroscope\n");
        printf("%s\n", strerror(errno));
    }
}
