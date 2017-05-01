/*
 * 4gewinnt-Konstanten.h
 *
 *  Created on: 10.05.2015
 *      Author: simon
 */

#ifndef _4GEWINNT_KONSTANTEN_H_
#define _4GEWINNT_KONSTANTEN_H_

#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include "Output.h"
#include "gpioinput.h"

using namespace std;

#define ANZAHL_RASTER 7									//Anzahl der SpielfeldSpalten
#define CAPEMGR_PATH "/sys/devices/bone_capemgr.9"		//Pfad zum CapeManager
#define OCP_PATH "/sys/devices/ocp.3"					//Pfad zum Handler für alle Devices
#define MAX_BUF 64										//Buffergroesse

//handles fuer Eingaenge definieren; verbinde IN# mit zugegöhörigem Linux-PIN
#define IN1 66
#define IN2 67
#define IN3 69
#define IN4 68
#define IN5 45
#define IN6 44
#define IN7 23
#define IN8 26
#define IN9 47
#define IN10 46
#define IN11 27
#define IN12 65


//Handles fuer Ausgaenge definieren; Verbinde OUT# mit zugehörigem EIN-Wert für I2C-Expander
#define PWM1 "bone_pwm_P9_14"
#define PWM2 "bone_pwm_P9_16"
#define PWM_Cape "am33xx_pwm"
#define OUT3 0x80
#define OUT4 0x40
#define OUT5 0x20
#define OUT6 0x10
#define OUT7 0x08
#define OUT8 0x04
#define OUT9 0x02		//Motor Spule1
#define OUT10 0x01		//Motor Spule2
#define OUT11 0x80		//Motor Spule3
#define OUT12 0x40		//Motor Spule4
#define RELAY1 0x20		//LED Spieler1
#define RELAY2 0x10		//LED Spieler2

//weitere Konstanten für den IO-Expander
#define DEVICE_ADDR	0x20		//Address of MCP23017 16-bit Output-Expander on I2C-Bus 1
#define OUTPUT_REG1	0x13		//hier liegen die Ausgaenge 3 bis 10
#define OUTPUT_REG2 0x14		//hier liegen die Ausgaenge 11, 12 und die Relais 1 und 2
#define MOTORDELAY 10000
#define AUSWURFZEIT 500000		//Zeit, die der hubmagnet angezogen bleibt	500ms




void shutdown(void);			//Faehrt BBB geordnet herunter
int blinken(Output* _wer, gpio_input* _ende, int _t_us);




#endif /* 4GEWINNT_KONSTANTEN_H_ */
