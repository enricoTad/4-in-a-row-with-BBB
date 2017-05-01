/*
 * PWM.cpp
 *
 *  Created on: 21.05.2015
 *      Author: simon
 */

#include "PWM.h"

PWM::PWM(unsigned int _PWMNummer)
/****************************************************************
 *\brief Konstruktor
 *
 *\param_in:	Nummer des PWM-Ausgangs auf dem BB-IO-Board
 *\param_out:	---
 */
{
	m_uPWMNummer = _PWMNummer;
	m_uStatus = 0;
	this->init();
	usleep(5000);
	this->setStatus(0);
}

PWM::~PWM()
/****************************************************************
 *\brief Destruktor
 *
 *\param_in:	---
 *\param_out:	---
 */
{
}

int PWM::init(void)
/****************************************************************
 *\brief initialisiert den CapeManager und legt den PWM-Ausgang im OCP an
 *
 *\param_in:	---
 *\param_out:	0, wenn fehlerfrei; !0, wenn fehler
 */
{
	int fd, len;
	char cache[MAX_BUF];
	fd = open(CAPEMGR_PATH "/slots", O_WRONLY);
	if(fd<0)
	{
		perror("PWM/open");
		cout << "unable to open CAPEMGR" << endl;
		return fd;
	}
	len = snprintf(cache, sizeof(cache), PWM_Cape);
	write(fd, cache, len);
	close (fd);
	usleep(500);
	fd = open(CAPEMGR_PATH "/slots", O_WRONLY);
	if(fd<0)
	{
		perror("PWM/open");
		cout << "unable to open CAPEMGR" << endl;
		return fd;
	}

	if(m_uPWMNummer == 1)
	{
		len = snprintf(cache, sizeof(cache), PWM1);
	}
	else if (m_uPWMNummer == 2)
	{
		len = snprintf(cache, sizeof(cache), PWM2);
	}

	write(fd, cache, len);
	close (fd);
	return 0;
}

unsigned int PWM::getStatus(void)
/****************************************************************
 *\brief gibt den Zustand des PWM-Ausgangs zurück
 *
 *\param_in:	---
 *\param_out:	0 = Low, 1 = High
 */
{
	getValue();
	return m_uStatus;
}

int PWM::setStatus(unsigned int _Status)
/****************************************************************
 *\brief Schaltet den PWM-Ausgang
 *
 *\param_in:	0 = Low, 1 = High
 *\param_out:	0, wenn fehlerfrei; !0, wenn fehler
 */
{
	int fd;
	char buf[MAX_BUF];
	if(m_uPWMNummer == 1)
	{
		snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/pwm_test_P9_14.13/run");
	}
	else if (m_uPWMNummer == 2)
	{
		snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/pwm_test_P9_16.12/run");
	}
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("PWM/setStatus");
		cout << "PWM ist #: " << m_uPWMNummer << endl;
		return fd;
	}
	if (_Status)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);
	close(fd);
	return 0;
}

int PWM::getValue(void)
/****************************************************************
 *\brief aktualisiert den Member m_uStatus
 *
 *\param_in:	---
 *\param_out:	0, wenn fehlerfrei, !0, wenn fehler
 */
{
	int fd;
	char buf[MAX_BUF];
	char ch;

	if(m_uPWMNummer == 1)
	{
		snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/pwm_test_P9_14.12/run");
	}
	else if (m_uPWMNummer == 2)
	{
		snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/pwm_test_P9_16.15/run");
	}

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		m_uStatus = 1;
	} else {
		m_uStatus = 0;
	}

	close(fd);
	return 0;
}
