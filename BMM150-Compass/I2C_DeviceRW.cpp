#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include <cmath>

#include <linux/i2c-dev.h>
// NEED IMPORT FIX
#include <i2c/smbus.h>
#ifndef I2C_M_RD
#include <linux/i2c.h>
#endif

using namespace std;

class I2cDevice
{
public:
	// Open device at slave address
	I2cDevice(unsigned char slaveAddress) : slaveAddress(slaveAddress)
	{
		const char *i2c_fname = "/dev/i2c-1";

		if ((file = open(i2c_fname, O_RDWR)) < 0)
		{
			char err[200];
			sprintf(err, "open('%s') in i2c_init", i2c_fname);
			perror(err);
		}
	}

	// Writes one byte to a register
	void writeRegister(unsigned char registerAddress, unsigned char val)
	{
		unsigned char outbuf[2];
		struct i2c_msg msgs[1];
		struct i2c_rdwr_ioctl_data msgset[1];

		// The register address to set and its value
		outbuf[0] = registerAddress;
		outbuf[1] = val;

		// Writes the data to the register
		msgs[0].addr = slaveAddress;
		msgs[0].flags = 0;
		msgs[0].len = 2;
		msgs[0].buf = outbuf;

		msgset[0].msgs = msgs;
		msgset[0].nmsgs = 1;

		if (ioctl(file, I2C_RDWR, &msgset) < 0)
		{
			perror("ioctl(I2C_RDWR) in i2c_write");
		}
	}

	// Reads data starting from a register address
	void readRegister(unsigned char registerAddress, unsigned char *bytes, int len)
	{
		/*
		unsigned char outbuf[1];
		struct i2c_msg msgs[2];
		struct i2c_rdwr_ioctl_data msgset[1];

		// We want to read this address
		outbuf[0] = registerAddress;

		// Specify which address to read
		msgs[0].addr = slaveAddress;
		msgs[0].flags = 0;
		msgs[0].len = 1;
		msgs[0].buf = outbuf;

		// Read the actual data
		msgs[1].addr = slaveAddress;
		msgs[1].flags = I2C_M_RD;
		msgs[1].len = len;
		msgs[1].buf = bytes;

		msgset[0].msgs = msgs;
		msgset[0].nmsgs = 2;

		if (ioctl(file, I2C_RDWR, &msgset) < 0)
		{
			perror("ioctl(I2C_RDWR) in i2c_write");
		}
		*/

		// NOT COMPLETE
		__u8 reg = registerAddress;

		/* Using SMBus commands */
		bytes = i2c_smbus_read_word_data(file, reg);
	}

private:
	int file;
	unsigned char slaveAddress;
};

// BMM150 device
class BMM150Compass : public I2cDevice
{
public:
	// Initialize the compass (Probably need to do some callibration here.
	BMM150Compass(unsigned char slaveAddress) : I2cDevice(slaveAddress)
	{
		// Set power control bit to 1
		writeRegister(0x4B, 1);
		// Delay for a few seconds to make sure we are ready.
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		// Set operation mode to normal
		writeRegister(0x4C, 0);
		// Delay for a few seconds to make sure we are ready.
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}

	float getAngle()
	{
		unsigned char data[8];
		// Read registers 0x42 through 0x49 (8 bytes in all)
		readRegister(0x42, data, 8);

		// Read X data (probably need to do some compensation here (see Arduino code)
		int16_t msb_data = ((int16_t)((int8_t)data[1])) * 32;
		int16_t raw_datax = (int16_t)(msb_data | data[0]);

		// Read Y data (probably need to do some compensation here (see Arduino code)
		msb_data = ((int16_t)((int8_t)data[3])) * 32;
		int16_t raw_datay = (int16_t)(msb_data | data[2]);

		int16_t output = std::atan2(raw_datax, raw_datay) * 180 / M_PI;
		// Calculate angle
		return output;
	}
};

int main()
{
	// Create compass at the slave address 0x13
	BMM150Compass compass(0x13);

	while (true)
	{
		// Print out the angle
		std::cout << compass.getAngle() << std::endl;
		// Delay a bit until next reading.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
