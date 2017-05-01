/*
 * Spielfeld.cpp
 *
 *  Created on: 19.05.2015
 *      Author: simon
 */

#include "Spielfeld.h"

Spielfeld::Spielfeld() 
{
}

Spielfeld::~Spielfeld() 
{
}

void Spielfeld::init(void)
/****************************************************************
 * vorbelegen aller Spielzellen mit 0
 *
 */
{
	for(int i = 0; i<ANZAHL_SPALTEN; i++)
	{
		for (int k = 0; k<ANZAHL_ZEILEN; k++)
		{
			m_aZellen[i][k].setStatus(0);
		}
	}
}
int Spielfeld::zug(unsigned int Spielertoken, unsigned int uSpaltennr)
/****************************************************************
 *\brief
 *
 *\param_in:	Spielertoken (1 oder 2), uSpaltennr Spalte, in der gesetzt werden soll
 *\param_out:	playertoken (1,2), if one of them won, 0 if undecided, -1 if no win- or undecided situation
 */
{
    int iSpielstand = -1;
    unsigned int uZeilennr = 0;
    bool bSpalteNichtPlausibel = true;
    bool bAbwurfNichtMoeglich = true;

    while (bSpalteNichtPlausibel || bAbwurfNichtMoeglich)
    {
		if ( (uSpaltennr >= 0) && (uSpaltennr <= 6) )
        //if ((uSpaltennr >= 1) && (uSpaltennr <=7))
        {
            // uSpaltennr--;	//auf Feldindex (0 bis 6) umrechnen
            bSpalteNichtPlausibel = false;
        }
        bAbwurfNichtMoeglich = !abwurfMoeglich(uSpaltennr, &uZeilennr);	//prüfe, ob setzt möglich und in welche zeile
    }

    //Zug setzen
    m_aZellen[uSpaltennr][uZeilennr].setStatus(Spielertoken);
    iSpielstand = wincheck(&Spielertoken);
    return iSpielstand;
}

int Spielfeld::wincheck(unsigned int* Spielertoken)
/****************************************************************
 *\brief	checks playing field for any win- or undecided situation
 *
 *\param_in:	---
 *\param_out:	player-token(1,2), if a player has won; 0 if undecided; -1, if no win or undecided situation
 */
{
    int iSpielstand =  0;

    if ((iSpielstand = checkWaagerecht(Spielertoken))>0)
    {
        return iSpielstand;
    }
    if ((iSpielstand = checkSenkrecht(Spielertoken))>0)
    {
        return iSpielstand;
    }
    if ((iSpielstand = checkDiagonal(Spielertoken))>0)
    {
        return iSpielstand;
    }
    if ((iSpielstand = checkRemis( ))==0)
    {
        return iSpielstand;
    }
    return -1;
}

int Spielfeld::checkWaagerecht(unsigned int* Spielertoken)
/****************************************************************
 *\brief checking for a win-situation of current player in horizontal direction
 *
 *\param_in:	---
 *\param_out:	-1, if no win-situation, number of player-token (1,2), if win-situation for a player
 */
{
    int iZeilennr=0, iSpaltennr=0, i=0, iZaehler=0;

    for (iZeilennr=0; iZeilennr<6; iZeilennr++)
    {
        for (iSpaltennr=0; iSpaltennr<4; iSpaltennr++)
        {
            iZaehler = 0;
            for (i=0; i<4; i++)
            {
                if (*Spielertoken == m_aZellen[iSpaltennr+i][iZeilennr].getStatus())
                {
                    iZaehler++;
                }
                else
                {
                    break;
                }
            }
            if (iZaehler >= 4)
            {
                return *Spielertoken;
            }
        }
    }
    return -1;
}

int Spielfeld::checkSenkrecht(unsigned int* Spielertoken)
/****************************************************************
 *\brief checking for a win-situation of current player in vertical direction
 *
 *\param_in:	---
 *\param_out:	-1, if no win-situation, number of player-token (1,2), if win-situation for a player
 */
{
    int iZeilernnr=0, iSpaltennr=0, i=0, iZaehler=0;

    for (iSpaltennr=0; iSpaltennr<7; iSpaltennr++)
    {
        for (iZeilernnr=0; iZeilernnr<3; iZeilernnr++)
        {
            iZaehler = 0;
            for (i=0; i<4; i++)
            {
                if (*Spielertoken==m_aZellen[iSpaltennr][iZeilernnr+i].getStatus())
                {
                    iZaehler++;
                }
                else
                {
                    break;
                }
            }
            if (iZaehler >= 4)
            {
                return *Spielertoken;
            }
        }
    }
    return -1;
}

int Spielfeld::checkDiagonal(unsigned int* Spielertoken)
/****************************************************************
 *\brief checking for a win-situation of current player in diagonal direction
 *
 *\param_in:	---
 *\param_out:	-1, if no win-situation, number of player-token (1,2), if win-situation for a player
 */
{
    int iZeilernnr=0, iSpaltennr=0;

    //Richtung rechts oben
    for (iZeilernnr=0; iZeilernnr<3; iZeilernnr++)
    {
        for (iSpaltennr=0; iSpaltennr<4; iSpaltennr++)
        {
            if  ( (*Spielertoken		                             == m_aZellen[iSpaltennr][iZeilernnr].getStatus())     && \
                  (m_aZellen[iSpaltennr][iZeilernnr].getStatus()     == m_aZellen[iSpaltennr+1][iZeilernnr+1].getStatus()) && \
                  (m_aZellen[iSpaltennr+1][iZeilernnr+1].getStatus() == m_aZellen[iSpaltennr+2][iZeilernnr+2].getStatus()) && \
                  (m_aZellen[iSpaltennr+2][iZeilernnr+2].getStatus() == m_aZellen[iSpaltennr+3][iZeilernnr+3].getStatus())
                )
            {
                return *Spielertoken;
            }
        }
    }

    //Richtung rechts unten
    for (iZeilernnr=3; iZeilernnr<5; iZeilernnr++)
    {
        for (iSpaltennr=0; iSpaltennr<4; iSpaltennr++)
        {
            if  ((*Spielertoken			                            == m_aZellen[iSpaltennr][iZeilernnr].getStatus())    && \
                 (m_aZellen[iSpaltennr][iZeilernnr].getStatus()     == m_aZellen[iSpaltennr+1][iZeilernnr-1].getStatus()) && \
                 (m_aZellen[iSpaltennr+1][iZeilernnr-1].getStatus() == m_aZellen[iSpaltennr+2][iZeilernnr-2].getStatus()) && \
                 (m_aZellen[iSpaltennr+2][iZeilernnr-2].getStatus() == m_aZellen[iSpaltennr+3][iZeilernnr-3].getStatus())
                 )
            {
                return *Spielertoken;
            }
        }
    }

    return -1;
}

int Spielfeld::checkRemis(void)
/****************************************************************
 *\brief  checks playing field if any cell is left empty
 *
 *\param_in:	---
 *\param_out:	0, if all cells are filled by a playertoken, -1, if one or more cells are still empty
 */
{
    for (int i=0; i<7; i++)
    {
        if (m_aZellen[i][5].getStatus()==0) //checking top row
        {
            return -1;
        }
    }
    return 0;
}

bool Spielfeld::abwurfMoeglich(unsigned int uSpaltennr, unsigned int* uZeile)
/****************************************************************
 *\brief  checks playing field if selected column is not completely filled
 *
 *\param_in:	---
 *\param_out:	true, if a  cell is left empty; false, if not
 *\param_out:	uZeile enthält, wohin gesetzt werden kann
 */
{
    unsigned int uZeilennr = 0;
	bool bVoll;

	//Spalte voll???
    for (uZeilennr = 0; uZeilennr < 6; uZeilennr++)
    {
        if (m_aZellen[uSpaltennr][uZeilennr].getStatus() == 0)
        {
            bVoll = false;
            *uZeile = uZeilennr;
            break;
        }
    }
    return !bVoll;
}
