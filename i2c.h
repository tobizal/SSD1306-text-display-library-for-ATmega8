#ifndef I2C_MODE_H
#define I2C_MODE_H
typedef enum {
	READ = 1, 
	WRITE = 0
} I2C_MODE;
#endif

// API
void i2c_start();
void i2c_send_address(int address, I2C_MODE mode);
void i2c_send_byte(int data);
void i2c_stop();
void error(int code);
