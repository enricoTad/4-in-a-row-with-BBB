/*
 * gpioinput.cpp
 *
 *  Created on: 13.05.2015
 *      Author: simon
 */

#include "gpioinput.h"


int gpio_input::gpio_export()
/****************************************************************
 *\brief Anlegen des GPIO-Ports im User-Space
 *
 *\param_in:	---
 *\param_out:	0, wenn fehlerfrei; !0, wenn fehler
 */
{
	int filedescriptor, len;
	char buffer[MAX_BUF];

	filedescriptor = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (filedescriptor < 0) {
		perror("gpio/export");
		return filedescriptor;
	}

	len = snprintf(buffer, sizeof(buffer), "%d", m_uPinNummer);
	write(filedescriptor, buffer, len);
	close(filedescriptor);

	return 0;
}

int gpio_input::gpio_unexport()
/****************************************************************
 *\brief zuruecklegen des GPIO-Ports in den Kernel-Space
 *
 *\param_in:	---
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	int filedescriptor, len;
	char buf[MAX_BUF];

	filedescriptor = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (filedescriptor < 0) {
		perror("gpio/export");
		return filedescriptor;
	}

	len = snprintf(buf, sizeof(buf), "%d", m_uPinNummer);
	write(filedescriptor, buf, len);
	close(filedescriptor);
	return 0;
}

int gpio_input::gpio_set_dir(unsigned int out_flag)
/****************************************************************
 *\brief Konfiguration als Eingang oder Ausgang
 *
 *\param_in:	0 = eingang, 1 = ausgang
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	int filedescriptor;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", m_uPinNummer);

	filedescriptor = open(buf, O_WRONLY);
	if (filedescriptor < 0) {
		perror("gpio/direction");
		return filedescriptor;
	}

	if (out_flag)
		write(filedescriptor, "out", 4);
	else
		write(filedescriptor, "in", 3);

	close(filedescriptor);
	return 0;
}

int gpio_input::gpio_set_value(unsigned int _Value)
/****************************************************************
 *\brief Schaltet den Ausgang auf _Value
 *
 *\param_in:	_Value: 0 = Low, 1 = High
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	int filedescriptor;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", m_uPinNummer);

	filedescriptor = open(buf, O_WRONLY);
	if (filedescriptor < 0) {
		perror("gpio/set-value");
		return filedescriptor;
	}

	if (_Value)
		write(filedescriptor, "1", 2);
	else
		write(filedescriptor, "0", 2);

	close(filedescriptor);
	return 0;
}

int gpio_input::gpio_set_edge(char *_Edge)
/****************************************************************
 *\brief Konfiguration der Flankenerkennung
 *
 *\param_in:	edge == "none": keine Flankenerkennung
 *\param_in:	edge == "rising": flankenerkennung steigend
 *\param_in:	edge == "falling": flankenerkennung fallend
 *\param_in:	edge == "both": flankenerkennung steigend und fallend
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	int filedescriptor;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", m_uPinNummer);

	filedescriptor = open(buf, O_WRONLY);
	if (filedescriptor < 0) {
		perror("gpio/set-edge");
		return filedescriptor;
	}
	write(filedescriptor, _Edge, strlen(_Edge) + 1);
	close(filedescriptor);
	return 0;
}

int gpio_input::gpio_set_activity(unsigned int _ActiveLow)
/****************************************************************
 *\brief Konfiguration der Logik-pegel
 *
 *\param_in:	_ActiveLow: 0 = high-aktiv; 1 = Low-aktiv
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	int filedescriptor;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/active_low", m_uPinNummer);
	filedescriptor = open(buf, O_WRONLY);
	if (filedescriptor < 0) {
		perror("gpio/set-Activity");
		return filedescriptor;
	}

	if (_ActiveLow)
		write(filedescriptor, "1", 2);
	else
		write(filedescriptor, "0", 2);

	close(filedescriptor);
	return 0;
}

int gpio_input::gpio_get_value(unsigned int *_Value)
/****************************************************************
 *\brief Abfrage des Port-Zustands
 *
 *\param_in:	_Value: hier wird der Port-Zustand reingeschrieben
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	int filedescriptor;
	char buf[MAX_BUF];
	char ch;
	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", m_uPinNummer);
	filedescriptor = open(buf, O_RDONLY);
	if (filedescriptor < 0) {
		perror("gpio/get-_Value");
		return filedescriptor;
	}
	read(filedescriptor, &ch, 1);

	if (ch != '0') {
		*_Value = 1;
	} else {
		*_Value = 0;
	}
	close(filedescriptor);
	return 0;
}

gpio_input::gpio_input(unsigned int _PinNummer, unsigned int _directionFlag, unsigned int _activeLow, char* _edgetype)
/****************************************************************
 *\brief Konstruktor; Anlegen des Ports im User-Space und Konfiguration;
 *\brief ACHTUNG: default-Werte
 *
 *\param_in:	_PinNummer: Nummer mit der das Linux-OS arbeitet
 *\param_in:	_directionFlag: 0 = Eingang, 1 = Ausgang
 *\param_in:	_activeLow: 0 = Pin ist high-aktiv, 1 = ist low-aktiv
 *\param_in:	_edgetype: Flankenerkennung
 *\param_out:	---
 */
{
	m_uPinNummer = _PinNummer;
	m_uValue = 0;
	m_uDirection = _directionFlag;
	m_uActivity = _activeLow;
	m_sEdgetype = _edgetype;
	m_iFiledescriptor = 0;
	gpio_export();
	gpio_set_dir(m_uDirection);
	gpio_set_edge(_edgetype);
	gpio_set_activity(m_uActivity);
}

gpio_input::~gpio_input(void)
/****************************************************************
 *\brief DEstruktor; zuruecklegen des Ports in Kernel-Space
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	if(m_iFiledescriptor != 0)
	{
		gpio_fd_close(m_iFiledescriptor);
	}
	gpio_unexport();
}

int gpio_input::gpio_fd_open(void)
/****************************************************************
 *\brief OEffnen der Port-Datei, z.B. für Polling
 *
 *\param_in:	---
 *\param_out:	zum Port gehoeriger Filedescriptor
 */
{
		int filedescriptor;
		char buf[MAX_BUF];

		if(m_iFiledescriptor > 0)
		return m_iFiledescriptor;

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", m_uPinNummer);
	filedescriptor = open(buf, O_RDONLY | O_NONBLOCK );
	if (filedescriptor < 0) {
		perror("gpio/filedescriptor_open");
	}
	m_iFiledescriptor = filedescriptor;
	return filedescriptor;
}

int gpio_input::gpio_fd_close(int _Filedescriptor)
/****************************************************************
 *\brief Schließen des _Filedescriptors
 *
 *\param_in:	_Filedescriptor: zum Port gehoeriger Filedescriptor
 *\param_out:	0, wenn erfolgreich; !0, wenn fehler
 */
{
	m_iFiledescriptor = 0;
	return close(_Filedescriptor);
}

unsigned int gpio_input::getValue(void)
/****************************************************************
 *\brief Abfrage des Port-Zustands
 *
 *\param_in:	---
 *\param_out:	Portstatus: 0=low, 1=high
 */
{
	gpio_get_value(&m_uValue);
	return m_uValue;
}

string gpio_input::getEdgetype (void)
/****************************************************************
 *\brief Abfrage der Flankenerkennung
 *
 *\param_in:	---
 *\param_out:	"rising", "falling", "both"	: Flankenerkennung AN, entsprechend return
 *\param_out:	"none" : Flankenerkennung AUS
 */
{
	return m_sEdgetype;
}

unsigned int gpio_input::getActivity(void)
/****************************************************************
 *\brief Logik-Pegel-Einstellung abfragen
 *
 *\param_in:	---
 *\param_out:	0 = high-aktiv; 1 = low-aktiv
 */
{
	return m_uActivity;
}

int gpio_input::getFiledescriptor(void)
/****************************************************************
 *\brief Filedescriptor holen
 *
 *\param_in:	---
 *\param_out:	0, wenn Datei nicht geöffnet; Filesedcriptor, wenn datei offen, z.B. von Polling
 */
{
	if (m_iFiledescriptor == 0)
		return -1;
	return m_iFiledescriptor;
}

unsigned int gpio_input::getPinNummer(void)
/****************************************************************
 *\brief Abfrage der Nummer mit der Linux den Port handelt
 *
 *\param_in:	---
 *\param_out:	PinNummer, mit der Port angelegt wurde
 */
{
	return m_uPinNummer;
}

unsigned int gpio_input::gpio_input::getDirection (void)
/****************************************************************
 *\brief Portkonfiguration abfragen
 *
 *\param_in:	---
 *\param_out:	0, wenn Eingang; 1, wenn Ausgang
 */
{
	return m_uDirection;
}
