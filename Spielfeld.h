/*
 * Spielfeld.h
 *
 *  Created on: 19.05.2015
 *      Author: simon
 */

#ifndef SPIELFELD_H_
#define SPIELFELD_H_

#include "Spielzelle.h"

#define ANZAHL_SPALTEN 7
#define ANZAHL_ZEILEN 6

class Spielfeld 
{
private:
	//Member
	unsigned int m_uAktuellerSpieler;
	Spielzelle m_aZellen[ANZAHL_SPALTEN][ANZAHL_ZEILEN];
	//Methoden
	int checkWaagerecht(unsigned int* Spielertoken);
	int checkSenkrecht(unsigned int* Spielertoken);
	int checkDiagonal(unsigned int* Spielertoken);
	int checkRemis(void);
	int wincheck(unsigned int* Spielertoken);

public:
	void init (void );
	Spielfeld();
	virtual ~Spielfeld();
	int zug(unsigned int Spielertoken, unsigned int Spaltennummer);
	bool abwurfMoeglich(unsigned int uSpaltennr, unsigned int* uZeile);
};

#endif /* SPIELFELD_H_ */
