/*
 * gpioinput.hpp
 *
 *  Created on: 13.05.2015
 *      Author: simon
 */

#ifndef GPIOINPUT_H_
#define GPIOINPUT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <iostream>
using namespace std;

 /****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 500) //1,5 seconds 	TODO: evtl. Polling-Timeout-Zeit nachstellen
#define MAX_BUF 64
#define ANZAHL_INPUTS 12


class gpio_input
{
private:
	unsigned int m_uPinNummer;		//nummer unter der der GPIO-Port vom Linux aus erreichbar
	unsigned int m_uValue;			//Zustand des Eingangs: 0=Low; 1=High
	unsigned int m_uDirection;		//Richtung des GPIO-Ports: 0=Input; 1=Output
	unsigned int m_uActivity;		//0=High-Active, 1=Low-Active
	string m_sEdgetype;				//Flankenerkennung: "none", "rising", "falling", "both"
	int m_iFiledescriptor;			//Filedescriptor fuer Dateihandling

	int gpio_export();
	int gpio_unexport();
	int gpio_set_dir(unsigned int _OutFlag);
	int gpio_set_value(unsigned int _Value);
	int gpio_set_edge(char *_Edge);
	int gpio_set_activity(unsigned int _ActiveLow);
	int gpio_get_value(unsigned int *_Value);

public:
	gpio_input(unsigned int _PinNummer, unsigned int _DirectionFlag = 0, unsigned int _ActiveLow = 0, char* _Edgetype = ((char*)"rising") );
	virtual ~gpio_input(void);
	int gpio_fd_open(void);
	int gpio_fd_close(int _Filedescriptor);
	unsigned int getValue(void);
	string getEdgetype (void);
	unsigned int getActivity(void);
	int getFiledescriptor(void);
	unsigned int getPinNummer(void);
	unsigned int getDirection (void);
};

#endif /* GPIOINPUT_H_ */
