/*	Hauptprogramm f�r das 4Gewinnt Spiel
	Autor:	Silvia St�ckle
	Datum: 29.05.2015
*/

// --------------------- INCLUDES -------------------------
#include "4gewinnt-Konstanten.h"
#include "gpioinput.h"
#include "I2C.h"
#include "lineareinheit.h"
#include "Output.h"
#include "PWM.h"
#include "Spielfeld.h"
#include "Spielzelle.h"
#include <iostream>
#include <stdio.h>
using namespace std;

// ------------------- HAUPTPROGRAMM ----------------------
int main ( )
{
	// ******* EINGAENGE, AUSGAENGE UND VARIABLEN *********
	// I2C Bus
	I2C_2 bus(DEVICE_ADDR);

	// Eingaenge anlegen
	gpio_input NotAus(66);
	gpio_input TasterQuittieren(67,0,1,"falling");
	gpio_input TasterStart(69,0,1,"falling");
	gpio_input TasterReset(68,0,1,"falling");
	gpio_input TasterLinks(23,0,1,"falling");
	gpio_input TasterRechts(26,0,1,"falling");
	gpio_input TasterSpielsteinAbwerfen(47,0,1,"falling");
	gpio_input EndschalterLinks(46);
	gpio_input EndschalterRechts(27);
	gpio_input SensorBehaelterEingelegt(65,0,1,"falling");

	// Ausgaenge anlegen

	PWM reset(2);
	Output quittieren(&bus, OUTPUT_REG1, 7);
	Output anlageEin(&bus, OUTPUT_REG1, 6);
    Output schwarzenSteinHolen(&bus, OUTPUT_REG1, 5);
	Output weissenSteinHolen(&bus, OUTPUT_REG1, 4);
	Output steinAbwerfen(&bus, OUTPUT_REG1, 3);
	Output spielfeldLeeren(&bus, OUTPUT_REG1, 2);
    Output spule1(&bus, OUTPUT_REG1, 1), spule2(&bus, OUTPUT_REG1, 0), spule3(&bus, OUTPUT_REG2, 7), spule4(&bus, OUTPUT_REG2, 6);
	Output ledSpieler1(&bus, OUTPUT_REG2, 5);
	Output ledSpieler2(&bus, OUTPUT_REG2, 4);

    lineareinheit motor(&EndschalterLinks, &EndschalterRechts, &spule1, &spule2, &spule3, &spule4);

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// ------ Init, falls vorher was lief; alles auf 0 setzen-----------------------
    reset.setStatus(0);
    quittieren.SetValue(0);
    anlageEin.SetValue(0);
    schwarzenSteinHolen.SetValue(0);
    weissenSteinHolen.SetValue(0);
    steinAbwerfen.SetValue(0);
    spielfeldLeeren.SetValue(0);
    spule1.SetValue(0);
    spule2.SetValue(0);
    spule3.SetValue(0);
    spule4.SetValue(0);
    ledSpieler1.SetValue(0);
    ledSpieler2.SetValue(0);


//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



	// Spiel
	Spielfeld spiel;

	// Variablen
	int aktuellerSpieler = 0;
	int spielstand = 0;
	int a = 0; //gibt Tasten erst dann frei wenn sich ein Chip im Fach befindet
	

	// *************** INITIALISIERUNG **************
	// Auf Quittierung warten
/*	do
	{
		quittieren.setStatus(1);					// Lampe am Quittieren Taster solange einschalten ...
	} while ( TasterQuittieren.getValue( ) == 0 );	// ... bis quittieret wird
*/
	blinken(&quittieren, &TasterQuittieren, 500000);
	
	quittieren.SetValue(0);						// Lampe am Quittieren Taster wieder ausschalten
	steinAbwerfen.SetValue(1);
	usleep(50000);
	steinAbwerfen.SetValue(0);
	steinAbwerfen.SetValue(1);
	usleep(50000);
	steinAbwerfen.SetValue(0);
	spielfeldLeeren.SetValue(1);
	sleep(2);
	spielfeldLeeren.SetValue(0);


	do
	{
		// Anlage starten
		blinken(&anlageEin, &TasterStart, 200000 );
		spiel.init();

	/*	do
		{
			spiel.init();	//belege Spielfeld mit 0en
		} while ( TasterStart.getValue( ) == 0 );		// Tue nichts bis Start gedr�ckt wird
*/
		anlageEin.SetValue(1);			// Wenn gestartet wird, Lampe am Taster Start einschalten

		motor.Referenzfahrt( );

		cout << "referenzfahrt beendet" << endl;
		// ***************** Spielablauf *****************
		do	
		{

			// Aktuellen Spieler bestimmen und Lampen einschalten
			if ( aktuellerSpieler == 0 || aktuellerSpieler == 2 )
			{
				aktuellerSpieler = 1;
				ledSpieler1.SetValue(1);
				ledSpieler2.SetValue(0);
				cout << "Spieler1 ausgewaehlt" << endl;
			}
			else if ( aktuellerSpieler == 1 )
			{
				aktuellerSpieler = 2;
				ledSpieler1.SetValue(0);
				ledSpieler2.SetValue(1);
				cout << "Spieler2 ausgewaehlt" << endl;
			}

			// Stein aus Magazin abholen
			motor.fahreAbholposition();
			cout << "Auf Abholposition" << endl;

			if ( aktuellerSpieler == 1 )
			{
				weissenSteinHolen.SetValue(1);		// Hubmagnet am Magazin ausloesen
				usleep(50000);	//TODO: Zeit bestimmen
				weissenSteinHolen.SetValue(0);
				cout << "Stein von Spieler1 holen" << endl;
				a = 1;
			}
			if ( aktuellerSpieler == 2 )
			{
				schwarzenSteinHolen.SetValue(1);	// Hubmagnet am Magazin ausloesen
				usleep(50000);	//TODO: Zeit bestimmen
				schwarzenSteinHolen.SetValue(0);
				cout << "Stein von Spieler2 holen" << endl;
				a = 1;
			}

			// Spieler f�hrt Spalte an
			while ( a == 1)		// Bis Spieler abwerfen drueckt
			{
				if ( TasterRechts.getValue( ) == 1 )
				{
					motor.FahreRasteVomMotor( );
					usleep(500000);
					cout << "Fahre Raste nach rechts" << endl;
				}
				if ( TasterLinks.getValue ( ) == 1 )
				{
					motor.FahreRasteZumMotor( );
					usleep(500000);
					cout << "Fahre Raste nach links" << endl;
				}
				if (TasterSpielsteinAbwerfen.getValue() == 1 && spiel.abwurfMoeglich(motor.getRaste(), 0 ) == 1)
				{
					a = 0;
				}
			}
			cout << "Spieler hat Abwerfen gedrückt" << endl;
			

			// Spieler setzt Stein in Spalte ab
			spielstand = spiel.zug(aktuellerSpieler, motor.getRaste( ));
			steinAbwerfen.SetValue(1);
			usleep(50000);//TODO: Zeit bestimmen
			steinAbwerfen.SetValue(0);
			sleep(2);
			
		} while ( spielstand == -1 );		// solange keiner gewonnen hat bzw. das Spiel unentschieden endet
		
		// ************* Gewinnauswertung **************
		if ( spielstand == 1 )
		{				
			ledSpieler2.SetValue(0);
			while ( TasterReset.getValue( ) == 0 )
			{
				ledSpieler1.SetValue(1);		// Spieler 1 hat gewonnen
				usleep(500000);//TODO: Zeit bestimmen
				ledSpieler1.SetValue(0);
				usleep(500000);
			}
		}

		if ( spielstand == 2 )
		{
			ledSpieler1.SetValue(0);
			while ( TasterReset.getValue( ) == 0 )
			{
				ledSpieler2.SetValue(1);		// Spieler 2 hat gewonnen
				usleep(500000);//TODO: Zeit bestimmen
				ledSpieler2.SetValue(0);
				usleep(500000);
			}
		}
				
		if ( spielstand == 0 )
		{
			while ( TasterReset.getValue( ) == 0 )
			{
				ledSpieler1.SetValue(1);		// Spiel war unentschieden
				usleep(500000);//TODO: Zeit bestimmen
				ledSpieler1.SetValue(0);
				ledSpieler2.SetValue(1);
				usleep(500000);//TODO: Zeit bestimmen
				ledSpieler2.SetValue(0);
			}
		}

		// ************* erneut spielen? **************
		// Ja, nur reset druecken


			if ( SensorBehaelterEingelegt.getValue( ) == 1 /*&& TasterReset.getValue( ) == 1*/ )
			{
				spielfeldLeeren.SetValue(1);
				sleep(2);	//2sec öffnen
				spielfeldLeeren.SetValue(0);
			}

		aktuellerSpieler = 0;

	} while ( NotAus.getValue( ) == 0 );
		
	// Nein, reset UND NotAus druecken
	anlageEin.SetValue(0);		// Wenn beendet wird, Lampe am Taster Start wieder ausschalten
	ledSpieler1.SetValue(0);	// LED Baender wieder ausschalten
	ledSpieler2.SetValue(0);
	shutdown( );				// Linux definiert herunterfahren und BeagleBone abschalten
		
	return 0;
}
