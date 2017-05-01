/*
 * Spielzelle.h
 *
 *  Created on: 19.05.2015
 *      Author: simon
 */

#ifndef SPIELZELLE_H_
#define SPIELZELLE_H_

#include <iostream>
using namespace std;

class Spielzelle 
{
private:
	unsigned int m_uStatus;
	void init (void);
public:
	Spielzelle();
	virtual ~Spielzelle();
	void setStatus (unsigned int uStatus);
	unsigned int getStatus (void);
};

#endif /* SPIELZELLE_H_ */

