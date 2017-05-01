/*
 * I2C.cpp
 *
 *  Created on: 14.05.2015
 *      Author: simon
 */

#include "I2C.h"


//-----------------------***********Klasse zu Testzwecken************---------------------------------------
I2C::I2C() {
	g_i2cFile = 0;
	this->i2cOpen();

}

I2C::~I2C() {
	this->i2cClose();
}

//open the linux device
void I2C::i2cOpen(void)
{
	//cout << "beagle-i2c opening /dev/i2c-1" << endl;
	g_i2cFile = open("/dev/i2c-1", O_RDWR);
	if (g_i2cFile < 0)
	{
		perror("i2cOpen in myI2C::i2cOpen");
		exit(1);
	}
	else
	{
	//	cout << "opening successful!" << endl;
	}
}

//close the linux device
void I2C::i2cClose(void)
{
	close (g_i2cFile);
}

//set the I2C slave address for all subsequent I2C device transfers
void I2C::i2cSetAddress(unsigned char address)
{
	//cout << "beagle -i2c setting address 0x" << hex << (int)address << "..." << endl;
	if (ioctl(g_i2cFile, I2C_SLAVE, address) < 0)
	{
		perror("i2cSetAddress error on myI2C::i2cSetAddress");
		exit(1);
	}
	else
	{
	//	cout << "setting address successful!" << endl;
	}
}

void I2C::Send_I2C_Byte(unsigned char _DEVICE_ADDR, unsigned char Reg_ADDR, unsigned char Data)
{
	i2cSetAddress(_DEVICE_ADDR);
	//cout << "beagle-i2c writing 0x" << hex << (int)Data << " to 0x" << hex << (int)_DEVICE_ADDR << ", reg 0x" << hex << (int)Reg_ADDR << endl;
	I2C_WR_Buf[0] = Reg_ADDR;
	I2C_WR_Buf[1] = Data;

	if (write(g_i2cFile, I2C_WR_Buf, 2) != 2)
	{
		perror("Write Error in myI2C::Send_I2C_Byte");
	}
	else
	{
	//	cout << "sending byte successful ..." << endl;
	}
}

unsigned char I2C::Read_I2C_Byte(unsigned char _DEVICE_ADDR, unsigned char Reg_ADDR)
{
	I2C_WR_Buf[0] = Reg_ADDR;

	i2cSetAddress(_DEVICE_ADDR);
	if (write(g_i2cFile, I2C_WR_Buf, 1) != 1)
	{
		perror("Write Error in myI2C::Read_I2C_Byte");
	}
	i2cSetAddress(_DEVICE_ADDR);
	if(read(g_i2cFile, I2C_RD_Buf, 1) != 1)
	{
		perror("Read Error my I2C::Read_I2C_Byte");
	}
	return I2C_RD_Buf[0];
}

unsigned char I2C::Read_Multi_Byte(unsigned char _DEVICE_ADDR, unsigned char Reg_ADDR, size_t n)
{
	I2C_WR_Buf[0] = Reg_ADDR;

	i2cSetAddress(_DEVICE_ADDR);
	ssize_t s = write(g_i2cFile, I2C_WR_Buf, 1);
	if (s != 1)
	{
		cout << "Wanted to write " << 1 << "byte, but instead wrote " << s << "." << endl;
		perror("Write Error in myI2C::Read_Multi_Byte");
	}
	i2cSetAddress(_DEVICE_ADDR);
	ssize_t t = read(g_i2cFile, I2C_RD_Buf, n);
	if (t != n)
	{
		cout << "wanted to read " << n << " bytes, but instead got " << t << "." << endl;
		perror("Read Error in myI2C::Read_Multi_Byte");
	}

	return I2C_RD_Buf[0];
}


//--------------------**************Optimierte Klasse*************-------------------------------------------------
I2C_2::I2C_2(unsigned char _DeviceAddress)
/****************************************************************
 *\brief konstruktor: OEffnen und initialisieren des I2C-Bus
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	g_i2cFile = 0;
	m_DeviceAddress = _DeviceAddress;
	this->i2cOpen();
	this->i2cSetAddress();
	this->init();

}

I2C_2::~I2C_2()/****************************************************************
 *\brief Destruktor; Schließen des Filedescriptors
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	this->i2cClose();
}

//open the linux device
void I2C_2::i2cOpen(void)
/****************************************************************
 *\brief OEffnen der I2C-Device-Datei und ablegen des Filedescriptors in g_i2cFile
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	//cout << "beagle-i2c opening /dev/i2c-1" << endl;
	g_i2cFile = open("/dev/i2c-1", O_RDWR);
	if (g_i2cFile < 0)
	{
		perror("i2cOpen in myI2C::i2cOpen");
		exit(1);
	}
	else
	{
	//	cout << "opening successful!" << endl;
	}
}

//close the linux device
void I2C_2::i2cClose(void)
/****************************************************************
 *\brief Schließen der I2C-Device-Datei
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	close (g_i2cFile);
}


void I2C_2::i2cSetAddress(void)
/****************************************************************
 *\brief Initialisiert den Bus, um bei allen weiteren Operationen mit dem Slave <m_DeviceAddress> zu arbeiten
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	//cout << "beagle -i2c setting Address 0x" << hex << (int)m_DeviceAddress << "..." << endl;
	if (ioctl(g_i2cFile, I2C_SLAVE, m_DeviceAddress) < 0)
	{
		perror("i2cSetAddress error on myI2C::i2cSetAddress");
		exit(1);
	}
	else
	{
	//	cout << "setting m_DeviceAddress successful!" << endl;
	}
}

void I2C_2::Send_I2C_Byte(unsigned char _RegAddr, unsigned char _Data)
/****************************************************************
 *\brief Schreibt ein Byte auf dem Busteilnemer m_DeviceAddress
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	i2cSetAddress();
	//cout << "beagle-i2c writing 0x" << hex << (int)Data << " to 0x" << hex << (int)m_DeviceAddress << ", reg 0x" << hex << (int)_RegAddr << endl;
	I2C_WR_Buf[0] = _RegAddr;
	I2C_WR_Buf[1] = _Data;

	if (write(g_i2cFile, I2C_WR_Buf, 2) != 2)
	{
		perror("Write Error in myI2C::Send_I2C_Byte");
	}
	else
	{
	//	cout << "sending byte successful ..." << endl;
	}
}

unsigned char I2C_2::Read_I2C_Byte(unsigned char _RegAddr)
/****************************************************************
 *\brief Liest EIN Byte vom Bus
 *
 *\param_in:	_RegAddr: Register, das gelesen wird
 *\param_out:	Inhalt des zu lesenden Registers _RegAddr
 */
{
	I2C_WR_Buf[0] = _RegAddr;

	i2cSetAddress();
	if (write(g_i2cFile, I2C_WR_Buf, 1) != 1)
	{
		perror("Write Error in myI2C::Read_I2C_Byte");
	}
	i2cSetAddress();
	if(read(g_i2cFile, I2C_RD_Buf, 1) != 1)
	{
		perror("Read Error my I2C::Read_I2C_Byte");
	}
	return I2C_RD_Buf[0];
}

unsigned char I2C_2::Read_Multi_Byte(unsigned char _RegAddr, size_t _n)
/****************************************************************
 *\brief Liest Bytes vom Bus
 *
 *\param_in:	_RegAddr: Adresse an der der Lesevorgand begonnen wird
 *\param_in:	_n: anzahl der Bytes, die gelesen werden
 *\param_out:	Anfangsadresse eines Char-Array[_n] in dem die gelesen Bytes liegen
 */
{
	I2C_WR_Buf[0] = _RegAddr;

	i2cSetAddress();
	ssize_t s = write(g_i2cFile, I2C_WR_Buf, 1);
	if (s != 1)
	{
		cout << "Wanted to write " << 1 << "byte, but instead wrote " << s << "." << endl;
		perror("Write Error in myI2C::Read_Multi_Byte");
	}
	i2cSetAddress();
	ssize_t t = read(g_i2cFile, I2C_RD_Buf, _n);
	if (t != _n)
	{
		cout << "wanted to read " << _n << " bytes, but instead got " << t << "." << endl;
		perror("Read Error in myI2C::Read_Multi_Byte");
	}

	return I2C_RD_Buf[0];
}

void I2C_2::setBit2Value(unsigned char _RegAddr, unsigned int _BitNummer, unsigned int _Value)
/****************************************************************
 *\brief Setzt des <_BitNummer>ste Bit im Byte <_RegAddr> auf den Wert <_Value>
 *
 *\param_in:	_RegAddr: Register, in dem ein Bit verändert werden soll
 *\param_in:	_BitNummer: Nummer det Bits (0..7) das geaendert werden soll
 *\param_in:	_Value: Wert auf den das beschrieben Bit gesetzt wird
 *\param_out:	---
 */
{
	unsigned char CharReadFromBus = this->Read_I2C_Byte(_RegAddr);
	unsigned int IntReadFromBus = int(CharReadFromBus);
	unsigned int IntWrite2Bus = 0;
	unsigned int mask;
	if (_Value)	// Setzte High
	{
		mask = 1 << _BitNummer;
		IntWrite2Bus = IntReadFromBus | mask;
	}
	else		// Setzte Low
	{
		mask = 0xFF - (1 << _BitNummer);
		IntWrite2Bus = IntReadFromBus & mask;
	}
	this->Send_I2C_Byte(_RegAddr, IntWrite2Bus);
}

void I2C_2::init(void)
/****************************************************************
 *\brief initialisierung des I2CBus für den I/O-Expander des BB-I/O-Boards; einrichten fuer Ausgaenge
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	this->Send_I2C_Byte(0x00, 0x00);
	this->Send_I2C_Byte(0x01, 0x00);
}
