#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <cstdio>
#include "raspi-bmm150_defs.h"

int main() {
	__uint8_t file_i2c;
	int length;
	unsigned char buffer[60];

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return -1;
	}
	
	//int addr = 0x13;          //<<<<<The I2C address of the slave (BMM150)
	
	//was "if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)"
	if (ioctl(file_i2c, I2C_SLAVE, BMM150_I2C_Address) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return -1;
	} else {
		printf("Communicated with I2C device. Returned: %u\n", file_i2c);
	}

	suspend_to_sleep_mode();

	write_op_mode(BMM150_SLEEP_MODE);
	
	//Check if same as Arduino program (it's not)
	if (file_i2c == BMM150_CHIP_ID) {
		printf("Connected to same device register as Arduino sample code");
	}

	//----- READ BYTES -----
	length = 16;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
        //output is 255 255 255 255 
        //NEED TO INVESTIGATE (make handshake)
		for (int i=0; i<length; i++) {
			printf("Data read: %d\n", buffer[i]);
		}
	}



	/*
	//----- WRITE BYTES -----
	buffer[0] = 0x01;
	buffer[1] = 0x02;
	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed 
		printf("Failed to write to the i2c bus.\n");
	} */

}

void suspend_to_sleep_mode() {
	set_power_control_bit(BMM150_POWER_CNTRL_ENABLE);
	//delay 3ms
	//delay(3);
}

void set_power_control_bit(__uint8_t pwrcntrl_bit) {
	//NOT DONE
	
	__uint8_t reg_data = 0;

	/* Power control register 0x4B is read */
    reg_data = i2c_read(BMM150_POWER_CONTROL_ADDR);
	//reg_data = read(file_i2c, buffer, 1);
    /* Sets the value of power control bit */
    reg_data = BMM150_SET_BITS_POS_0(reg_data, BMM150_PWR_CNTRL, pwrcntrl_bit);
    //i2c_write(BMM150_POWER_CONTROL_ADDR, reg_data);
}

void write_op_mode(__uint8_t op_mode) {
	
	__uint8_t reg_data = 0;
    
	//  #define BMM150_OP_MODE_ADDR		    0x4C
    reg_data = i2c_read(BMM150_OP_MODE_ADDR);

    /* Set the op_mode value in Opmode bits of 0x4C */
    reg_data = BMM150_SET_BITS(reg_data, BMM150_OP_MODE, op_mode);
    /**
	
	--------

     * /**name Macro to SET and GET BITS of a register
    #define BMM150_SET_BITS(reg_data, bitname, data) \
        ((reg_data & ~(bitname##_MSK)) | \
        ((data << bitname##_POS) & bitname##_MSK))
     *
     */
	

    //  #define BMM150_OP_MODE_ADDR		    0x4C
    i2c_write(BMM150_OP_MODE_ADDR, reg_data);
}

void i2c_read(short address, int8_t* buffer, short length) {
	//----- READ BYTES ----- //
	if (read(address, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
        //output is 255 255 255 255 
        //NEED TO INVESTIGATE (make handshake)
		for (int i=0; i<length; i++) {
			printf("Data read: %d\n", buffer[i]);
		}
	}
}

void i2c_read(short address) {
	uint8_t byte;

	//----- READ BYTES ----
	if (read(address, byte, 1) != 1)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %d\n", byte);
	}
}
