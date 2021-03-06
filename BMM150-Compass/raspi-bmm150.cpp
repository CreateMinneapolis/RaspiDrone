// Code from TFMini I2C

//to test for conected I2C devices, run: "sudo i2cdetect -y 1"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>

#include <linux/i2c-dev.h>
#ifndef I2C_M_RD
    #include <linux/i2c.h>
#endif

//add usr directory
#include <raspi-bmm150_defs.h>

using namespace std;

int main()
{
    int i2c_fd;
    const char *i2c_fname = "/dev/i2c-1";

    if ((i2c_fd = open(i2c_fname, O_RDWR)) < 0)
    {
        char err[200];
        sprintf(err, "open('%s') in i2c_init", i2c_fname);
        perror(err);
        return -1;
    }

    int retval;
    unsigned char outbuf[1];
    unsigned char inbuf[8];

    // BMM150 uses the same default I2C address (if not working, try 0x13)
    unsigned char slave_addr = 0x10;

    struct i2c_msg msgs[2];
    struct i2c_rdwr_ioctl_data msgset[1];

    // MIGHT NEED TO CHANGE
    outbuf[0] = 0x42;
    /*outbuf[1] = 2;
    outbuf[2] = 7; */

    msgs[0].addr = slave_addr;
    msgs[0].flags = 0;
    // send 1 byte
    msgs[0].len = 1;
    msgs[0].buf = outbuf;

    msgs[1].addr = slave_addr;
    msgs[1].flags = I2C_M_RD; // || add maybe I2C_STOP...;
    msgs[1].len = 8;
    msgs[1].buf = inbuf;

    msgset[0].msgs = msgs;
    msgset[0].nmsgs = 2;

    //needs rewriting
    while (true)
    {
        if (ioctl(i2c_fd, I2C_RDWR, &msgset) < 0)
        {
            perror("ioctl(I2C_RDWR) in i2c_write");
            return -1;
        }

        std::cout << "Dist " << ((inbuf[3] << 8 | inbuf[2])) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
