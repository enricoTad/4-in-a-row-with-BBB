/*
 * lineareinheit.h
 *
 *  Created on: 22.05.2015
 *      Author: simon
 */

#ifndef LINEAREINHEIT_H_
#define LINEAREINHEIT_H_
#include "Output.h"
#include "gpioinput.h"
#include "4gewinnt-Konstanten.h"

#define WARTEZEIT 5200 // 5ms Wartezeit zwischen den Schritten
#define WAIT2 0			// Wartezeit für Halbschritt => nicht verwendet = 0

class lineareinheit {
private:
	gpio_input* m_EndschalterAmEnde;			//Pointer zum Endschalter am Ende der Lineareinheit
	gpio_input* m_EndschalterAmMotor;			//Pointer zum Endschalter am Motor
	Output* m_Q9;								//Spule1 des Schrittmotors
	Output* m_Q10;								//Spule2 des Schrittmotors
	Output* m_Q11;								//Spule3 des Schrittmotors
	Output* m_Q12;								//Spule4 des Schrittmotors
	unsigned int m_uStep;						//StepNummer 0..3 für Schaltfolge der Spulen
	unsigned int m_uAktuellerSchrittwert;		//Aktuelle Position des Montageblocks in Schritten (0..m_uGesamterSchrittwert)
	unsigned int m_uAktuelleRaste;				//Aktuelle Rastposition (0..6)
	unsigned int m_uGesamterSchrittwert;		//maximale Position des Montageblocks in Schritten

	int StepVomMotorWeg();						//Bewegt den Montageblock EINEN Schritt vom Motor weg
	int StepZumMotorHin(); 						//Bewegt den Montageblock EINEN Schritt zum Motor hin
	void bewegen();								//Schaltet die Spulen in Abhängigkeit von m_uStep
	void bestimmeRastmarken();					//berechnet die Schrittwerte der Rastpositionen und fällt m_auRastmarke
	void spulenAbschalten();
public:
	lineareinheit(gpio_input* _EndschalterAmMotor, gpio_input* _EndschalterAmEndeDerLineareinheit, Output* _Spule1, Output* _Spule2, Output* _Spule3, Output* _Spule4);
	virtual ~lineareinheit();
	unsigned int getRaste();					//abfrage der aktuellen Raste
	unsigned int getAktuellerSchrittwert();		//abfrage der aktuellen Position in Schritten
	unsigned int getGesamterSchrittwert();		//Abfrage des möglichen Verfahrwegs in Schritten
	int FahreRasteZumMotor();					//bewegt montageblock EINE Raste näher zum Motor hin
	int FahreRasteVomMotor();					//bewegt Montageblock EINE Raste weiter vom Motor weg
	int FahreSteps(int _nSteps);				//ändert den Abstand des Montageblocks zum Motor um _nSteps (+ oder -)
	int Referenzfahrt();						//durchführen der Referenzfahrt
	unsigned int m_auRastmarke[ANZAHL_RASTER];	//Array, das die Schrittwerte der Rastositionen enthaelt
	void fahreAbholposition (void);
	void holeSpielstein(Output* _Aktor);
};

#endif /* LINEAREINHEIT_H_ */
