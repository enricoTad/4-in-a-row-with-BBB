/*
 * Output.h
 *
 *  Created on: 22.05.2015
 *      Author: simon
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_
#include "I2C.h"

class Output {
private:
	unsigned int m_uValue;				//Zustand des Ausgangs
	unsigned int m_uRegAddr;			//Adresse des Ausgangsbytes, das für diesen Output zuständig ist
	unsigned int m_uBitNummer;			//Nummer des Bits im Byte m_uRegAddr, das den Zustand des Ausgang mapt
	I2C_2* m_ptr_Bus;					//Poiter zum Ausgangsbus vom Typ I2C
public:
	Output(I2C_2* _BusPtr, unsigned int _RegAddr, unsigned int _BitNummer);	//konstruktor
	virtual ~Output(void);				//Destruktor
	void SetValue(unsigned int _Value);	//Ausgang auf Zustand _Value schalten
	unsigned int getValue(void);		//zustand des Ausgangs abfragen
};

#endif /* OUTPUT_H_ */

//BEACHTE:
//OUT3 == 0x13	Bit 7
//OUT4 == 0x13	Bit 6
//OUT5 == 0x13	Bit 5
//OUT6 == 0x13	Bit 4
//OUT7 == 0x13	Bit 3
//OUT8 == 0x13	Bit 2
//OUT9 == 0x13	Bit 1
//OUT10 == 0x13	Bit 0
//OUT11 == 0x14	Bit 7
//OUT12 == 0x14	Bit 6
//REL1 == 0x14	Bit 5
//REL2 == 0x14	Bit 4
