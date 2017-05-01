/*
 * I2C.h
 *
 *  Created on: 14.05.2015
 *      Author: simon
 */

#ifndef I2C_H_
#define I2C_H_

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

#define MAX_BUFFER_SIZE		64


//---------------------Klasse zu testzwecken----------------------------------------------------
class I2C {
private:

public:
	int g_i2cFile;
	I2C();
	virtual ~I2C();
	//public Variables
	unsigned char I2C_WR_Buf[MAX_BUFFER_SIZE];		//data you want to send
	unsigned char I2C_RD_Buf[MAX_BUFFER_SIZE];		//data which was read

	//Initialize functions
	void i2cOpen(void);								//open i2cbus, done at the beginning
	void i2cClose(void);							//closes i2cbus, done at the ending
	void i2cSetAddress(unsigned char address);		//changes device address

	//Sends a single byte <DATA> to <DEVICE_ADDR> on the register <Reg_ADDR>
	void Send_I2C_Byte(unsigned char _DEVICE_ADDR, unsigned char Reg_ADDR, unsigned char Data);

	//Reads and returns a single byte from <DEVICE_ADDR> in the register <Reg_ADDR>
	unsigned char Read_I2C_Byte(unsigned char _DEVICE_ADDR, unsigned char Reg_ADDR);

	//Reads multiples byte from >DEVICE_ADDR> starting from the register address <Reg_ADDR>.
	//Read the output from i2cptr->I2C_RD_Buf
	unsigned char Read_Multi_Byte(unsigned char _DEVICE_ADDR, unsigned char Reg_ADDR, size_t n);
};


//------------------------------------Klasse "I2C" optimiert----------------------------------------------------------
class I2C_2 {
private:
	int g_i2cFile;									//Filedescriptor für Bus-Datei
	unsigned char m_DeviceAddress;					//Addresse des anzusprechenden Slaves; hier 0x20
	unsigned char I2C_WR_Buf[MAX_BUFFER_SIZE];		//data you want to send on the bus
	unsigned char I2C_RD_Buf[MAX_BUFFER_SIZE];		//data which was read from bus
	//Initialize functions
	void i2cOpen(void);								//open i2cbus, done at the beginning
	void i2cClose(void);							//closes i2cbus, done at the ending
	void i2cSetAddress(void);						//changes device address
	void init(void);								//intitialisiert den I/O-Expander für Outputs
public:

	I2C_2(unsigned char _DeviceAddress);
	virtual ~I2C_2();

	//Sends a single byte <_Data> to m_DeviceAddress on the register <_RegAddr>
	void Send_I2C_Byte(unsigned char _RegAddr, unsigned char _Data);

	//Reads and returns a single byte from m_DeviceAddress in the register <_RegAddr>
	unsigned char Read_I2C_Byte(unsigned char Reg_ADDR);

	//Reads <_n> byte from m_DeviceAddress starting from the register address <_RegAddr>.
	//Read the output from i2cptr->I2C_RD_Buf
	unsigned char Read_Multi_Byte(unsigned char _RegAddr, size_t _n);

	//Sets 1 Bit (the <_BitNummer>-Bit) of the <_RegAddr>-Byte to <_Value> without overwriting the other Bits
	void setBit2Value(unsigned char _RegAddr, unsigned int _BitNummer, unsigned int _Value);
};

#endif /* I2C_H_ */


