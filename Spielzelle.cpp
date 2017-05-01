/*
 * Spielzelle.cpp
 *
 *  Created on: 19.05.2015
 *      Author: simon
 */

#include "Spielzelle.h"

Spielzelle::Spielzelle() {
	init();

}

Spielzelle::~Spielzelle() {

}


unsigned int Spielzelle::getStatus()
/****************************************************************
 *\brief returns token of the playing field
 *
 *\param_in:	---
 *\param_out:	playertoken = 0,1,2
 */
{
	return m_uStatus;
}

void Spielzelle::setStatus(unsigned int uStatus)
/****************************************************************
 *\brief setting a player-specific token to a playing field
 *
 *\param_in:	playertoken to be set
 *\param_out:	---
 */
{
	switch (uStatus) {
		case 0:	//field empty = no token
		case 1:	//token of player1
		case 2:	//token of player2
			m_uStatus = uStatus;	//set playertoken to status
			break;
		default:
			cout << "Der Status " << uStatus << "ist ungültig" << endl;
			cout << "Setzte Status auf 0" << endl;
			m_uStatus = 0;
 			break;
	}
}

void Spielzelle::init(void)
/****************************************************************
 *\brief initializes playing field with 0
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	setStatus(0);
}
