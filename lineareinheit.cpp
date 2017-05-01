/*
 * lineareinheit.cpp
 *
 *  Created on: 22.05.2015
 *      Author: simon
 */

#include "lineareinheit.h"

lineareinheit::lineareinheit(gpio_input* _EndschalterAmMotor, gpio_input* _EndschalterAmEndeDerLineareinheit,  Output* _Spule1, Output* _Spule2, Output* _Spule3, Output* _Spule4)
/****************************************************************
 *\brief Kontruktor; initialisieren der Linerareinheit
 *
 *\param_in:	_EndeRechts	=	Pointer zum Input des Endschalters am Ende der Lineareinheit
 *\param_in:	_EndeLinks	= 	Pointer zum Input des Endschalters am Motor
 *\param_in:	_Spule1		=	Ausgang für Spule1
 *\param_in:	_Spule2		=	Ausgang für Spule2
 *\param_in:	_Spule3		=	Ausgang für Spule3
 *\param_in:	_Spule4		=	Ausgang für Spule4
 *\param_out:	---
 */
{
	m_EndschalterAmMotor = _EndschalterAmMotor;
	m_EndschalterAmEnde = _EndschalterAmEndeDerLineareinheit;
	m_Q9 = _Spule1;
	m_Q10 = _Spule2;
	m_Q11 = _Spule3;
	m_Q12 = _Spule4;
	m_uStep = 0;
	m_uAktuellerSchrittwert = 0;
	m_uAktuelleRaste = 0;
	m_uGesamterSchrittwert = 0;
}

lineareinheit::~lineareinheit()
/****************************************************************
 *\brief Destruktor: only to be not defined by default
 *
 *\param_in:	---
 *\param_out:	---
 */
{

}

int lineareinheit::StepVomMotorWeg()
/****************************************************************
 *\brief bewegt Montageblock EINEN Schritt zum Motor hin
 *
 *\param_in:	---
 *\param_out:	0=fehlerfrei; -1 = Endlage erreicht
 */
{
	if(!m_EndschalterAmEnde->getValue())
	{
		if(m_uStep >= 3)
			m_uStep = 0;
		else
			m_uStep++;
		m_uAktuellerSchrittwert++;
		this->bewegen();
		return 0;	//alles lief gut
	}
	else
	{
		m_uAktuelleRaste = 6;
		return -1;	//endschalter ist betätigt, es wird nicht gefahren
	}
}

int lineareinheit::StepZumMotorHin()
/****************************************************************
 *\brief bewegt Montageblock EINEN Schritt vom Motor weg
 *
 *\param_in:	---
 *\param_out:	0 = fehlerfrei; -1 = Endlage erreicht
 */
{
	if(!m_EndschalterAmMotor->getValue())
	{
		if(m_uStep <= 0)
			m_uStep = 3;
		else
			m_uStep--;
		m_uAktuellerSchrittwert--;
		this->bewegen();
		return 0;
	}
	else
		return -1;

}

void lineareinheit::bewegen()
/****************************************************************
 *\brief in Anhängigkeit von m_uSteps wird das passende Schrittmuster auf die Ausgänge geschalten
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	switch (m_uStep)
	{
		case 0:
			m_Q9->SetValue(1);
			m_Q10->SetValue(1);
			m_Q11->SetValue(0);
			m_Q12->SetValue(0);
			usleep(WARTEZEIT);
			break;

		case 1:
			m_Q9->SetValue(0);
			m_Q10->SetValue(1);
			m_Q11->SetValue(1);
			m_Q12->SetValue(0);
			usleep(WARTEZEIT);
			break;

		case 2:
			m_Q9->SetValue(0);
			m_Q10->SetValue(0);
			m_Q11->SetValue(1);
			m_Q12->SetValue(1);
			usleep(WARTEZEIT);
			break;

		case 3:
			m_Q9->SetValue(1);
			m_Q10->SetValue(0);
			m_Q11->SetValue(0);
			m_Q12->SetValue(1);
			usleep(WARTEZEIT);
			break;
	}

}

void lineareinheit::bestimmeRastmarken()
/****************************************************************
 *\brief berrechnen der Schrittwerte für Rastpositionen und füllen von m_auRastmarke
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	for(int i = 0; i < ANZAHL_RASTER; i++)
	{
		m_auRastmarke[i] = (i * m_uGesamterSchrittwert) / 6;
	}



/*	m_auRastmarke[0] = 0;
	m_auRastmarke[1] = m_uGesamterSchrittwert * 1/6;
	m_auRastmarke[2] = m_uGesamterSchrittwert * 2/6;
	m_auRastmarke[3] = m_uGesamterSchrittwert * 3/6;
	m_auRastmarke[4] = m_uGesamterSchrittwert * 4/6;
	m_auRastmarke[5] = m_uGesamterSchrittwert * 5/6;
	m_auRastmarke[6] = m_uGesamterSchrittwert;
*/
}

unsigned int lineareinheit::getRaste()
/****************************************************************
 *\brief Abfrage der aktuellen Raste
 *
 *\param_in:	---
 *\param_out:	aktuelle Rastposition (0..6)
 */
{
	return m_uAktuelleRaste;
}

unsigned int lineareinheit::getAktuellerSchrittwert()
/****************************************************************
 *\brief Abfrage der Aktuellen Position in Schritten
 *
 *\param_in:	---
 *\param_out:	Schrittwert der Aktuellen Position (0..m_uGEsamterSchrittwert)
 */
{
	return m_uAktuellerSchrittwert;
}

unsigned int lineareinheit::getGesamterSchrittwert()
/****************************************************************
 *\brief Abfrage des maximalen Verfahrwegs
 *
 *\param_in:	---
 *\param_out:	maximaler Verfahrweg in Schritten
 */
{
	return m_uGesamterSchrittwert;
}

int lineareinheit::FahreRasteZumMotor()
/****************************************************************
 *\brief bewegt Montageblock EINE Raste zum Motor hin
 *
 *\param_in:	---
 *\param_out:	Aktueller Rastenwert (0..6)
 */
{//Rastenwert erniedrigen
	if( !(m_EndschalterAmMotor->getValue()) )
	{
		while( (m_uAktuellerSchrittwert > (m_auRastmarke[ (this->getRaste())-1 ])) && !(m_EndschalterAmMotor->getValue()) )
		{
			this->StepZumMotorHin();
		}
		m_uAktuelleRaste--;
		m_uAktuellerSchrittwert = m_auRastmarke[this->getRaste()];
	}
	else
	{
		m_uAktuellerSchrittwert = 0;
		m_uAktuelleRaste = 0;
	}
	this->spulenAbschalten();
	return m_uAktuelleRaste;
}

int lineareinheit::FahreRasteVomMotor()
/****************************************************************
 *\brief Bewegt Montageblock EINE Raste vom Motor weg
 *
 *\param_in:	---
 *\param_out:	Aktueller Rastenwert (0..6)
 */
{//Rastenwert erhöhen
	if( !(m_EndschalterAmEnde->getValue()) )
	{
		while( (m_uAktuellerSchrittwert < (m_auRastmarke[ (this->getRaste())+1 ])) && !m_EndschalterAmEnde->getValue() )
		{
			this->StepVomMotorWeg();
		}
		m_uAktuelleRaste++;
		m_uAktuellerSchrittwert = m_auRastmarke[this->getRaste()];
	}
	else
	{
		m_uAktuelleRaste = 6;
		m_uAktuellerSchrittwert = m_uGesamterSchrittwert;
	}
	this->spulenAbschalten();
	return m_uAktuelleRaste;
}

int lineareinheit::FahreSteps(int _nSteps)
/****************************************************************
 *\brief ändert Abstand des Montageblocks zum Motr (durch bewegen)
 *
 *\param_in:	_nSteps > 0: bewegt montageblock vom motor weg
 *\param_in:	_nSteps < 0: bewegt montageblock zum Motor hin
 *\param_out:	aktuelle position in Schritten
 */
{
	if(_nSteps > 0)	//bewege vom Motor weg
	{
		for(int i = 0; i < _nSteps; i++)
		{
			this->StepVomMotorWeg();
			if (this->StepVomMotorWeg()<0)
				break;
		}
	}
	else			//bewege zum Motor hin
	{
		for(int i = 0; i > _nSteps; i--)
		{
			this->StepZumMotorHin();
			if(this->StepZumMotorHin()<0)
				break;
		}
	}
	this->spulenAbschalten();
	return m_uAktuellerSchrittwert;
}

int lineareinheit::Referenzfahrt()
/****************************************************************
 *\brief durchfuehren der Refernzfahrt, abtasten der Linearachse, position und Verfahrweg bestimmen
 *
 *\param_in:	---
 *\param_out:	maximal moeglicher Verfahrweg
 */
{
	while( !(this->StepZumMotorHin()) );
	m_uAktuellerSchrittwert = 0;
	while( !(this->StepVomMotorWeg()) );
	m_uGesamterSchrittwert = m_uAktuellerSchrittwert;

	this->bestimmeRastmarken();
	m_uAktuelleRaste = 6;
/*	while(this->getRaste() != 3)
	{
		this->FahreRasteZumMotor();
	}*/
//	this->FahreRasteZumMotor();
	this->spulenAbschalten();
	return m_uGesamterSchrittwert;
}

void lineareinheit::spulenAbschalten()
/****************************************************************
 *\brief schaltet alle Motorspulen stromlos
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	m_Q9->SetValue(0);
	m_Q10->SetValue(0);
	m_Q11->SetValue(0);
	m_Q12->SetValue(0);
}

void lineareinheit::fahreAbholposition (void)
{
	while(this->getRaste() != 0)
		this->FahreRasteZumMotor();
	this->spulenAbschalten();
}

void lineareinheit::holeSpielstein(Output* _Aktor)
{
	this->fahreAbholposition();
	_Aktor->SetValue(1);
	usleep(AUSWURFZEIT);
	_Aktor->SetValue(0);
}
