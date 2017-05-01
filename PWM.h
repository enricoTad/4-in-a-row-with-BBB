/*
 * PWM.h
 *
 *  Created on: 21.05.2015
 *      Author: Simon Steinmayer
 */

#ifndef PWM_H_
#define PWM_H_
#include <iostream>
#include <stdio.h>
using namespace std;
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include "4gewinnt-Konstanten.h"

class PWM {
private:
	unsigned int m_uStatus;					//0=Low, 1=High
	int init(void);							//Initialisierung des CapeManagers für PWM
	unsigned int m_uPWMNummer;				//Nummer des Ausgangs vom BBIO, also 1 oder 2
	int getValue(void);						//aktualisiert m_uStatus
public:
	PWM(unsigned int _PWMNummer);			//Konstruktor
	virtual ~PWM(void);						//Destruktor
	int setStatus(unsigned int _Status);	//Schaltet den PWM-Ausgang auf Low oder High
	unsigned int getStatus(void);			//gibt den Status des Ausgangs (Low oder High) zurück
};

#endif /* PWM_H_ */
