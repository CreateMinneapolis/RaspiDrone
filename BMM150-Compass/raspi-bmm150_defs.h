#ifndef _BMM150_DEFS_H_
#define _BMM150_DEFS_H_

/** name API success code **/
#define BMM150_OK   (0)

#define BMM150_I2C_Address  0x13

/** CHIP ID & SOFT RESET VALUES      */
#define BMM150_CHIP_ID              0x32

/** Register Address */
#define BMM150_CHIP_ID_ADDR		    0x40
#define BMM150_DATA_X_LSB		    0x42
#define BMM150_DATA_X_MSB		    0x43
#define BMM150_DATA_Y_LSB		    0x44
#define BMM150_DATA_Y_MSB		    0x45
#define BMM150_DATA_Z_LSB		    0x46
#define BMM150_DATA_Z_MSB		    0x47
#define BMM150_DATA_READY_STATUS	0x48
#define BMM150_INTERRUPT_STATUS		0x4A
#define BMM150_POWER_CONTROL_ADDR	0x4B
#define BMM150_OP_MODE_ADDR		    0x4C
#define BMM150_INT_CONFIG_ADDR		0x4D
#define BMM150_AXES_ENABLE_ADDR		0x4E
#define BMM150_LOW_THRESHOLD_ADDR	0x4F
#define BMM150_HIGH_THRESHOLD_ADDR	0x50
#define BMM150_REP_XY_ADDR		    0x51
#define BMM150_REP_Z_ADDR		    0x52

#define BMM150_SLEEP_MODE		0x03

/**\name Power mode settings  */
#define	BMM150_POWER_CNTRL_DISABLE	0x00
#define	BMM150_POWER_CNTRL_ENABLE	0x01


#endif