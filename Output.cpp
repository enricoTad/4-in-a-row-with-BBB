/*
 * Output.cpp
 *
 *  Created on: 22.05.2015
 *      Author: simon
 */

#include "Output.h"

Output::Output(I2C_2* _BusPtr, unsigned int _RegAddr, unsigned int _BitNummer)
/****************************************************************
 *\brief only to be not defined by default
 *
 *\param_in:	_BusPtr = I2C-Bus, auf dem die ausgänge liegen
 *\param_in:	_RegAddr = Byte auf _BusPtr, welches zu diesem Ausgang gehört
 *\param_in:	_BitNummer = 0..7, Bit, welches den Ausgang darstellt
 *\param_out:	---
 */
{
	m_uBitNummer = _BitNummer;
	m_uRegAddr = _RegAddr;
	m_uValue = 0;
	m_ptr_Bus = _BusPtr;

}

Output::~Output()
/****************************************************************
 *\brief only to be not defined by default
 *
 *\param_in:	---
 *\param_out:	---
 */
{
}

void Output::SetValue(unsigned int _Value)
/****************************************************************
 *\brief Setzt den Ausgang auf _Value
 *
 *\param_in:	_Value = 0=Low; 1=High
 *\param_out:	---
 */
{
	m_ptr_Bus->setBit2Value(m_uRegAddr, m_uBitNummer, _Value);
	m_uValue = _Value;
}

unsigned int Output::getValue()
/****************************************************************
 *\brief Abfrage des Ausgangsstatuses
 *
 *\param_in:	---
 *\param_out:	0=Low; 1=High
 */
{
	unsigned int Value = int( m_ptr_Bus->Read_I2C_Byte(m_uRegAddr) );
	unsigned int mask = 1 << m_uBitNummer;
	Value = Value & mask;
	Value = Value >> m_uBitNummer;
	m_uValue = Value;

	return m_uValue;
}
