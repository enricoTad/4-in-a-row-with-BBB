#include "4gewinnt-Konstanten.h"
#include <poll.h>


void shutdown (void)
/****************************************************************
 *\brief 		geordnetes Herunterfahren von Linux, BeagleBone abschalten
 *
 *\param_in:	---
 *\param_out:	---
 */
{
	cout << "Schalte ab ..." << endl;
	system("shutdown -h now");
}


int blinken(Output* _wer, gpio_input* _ende, int _t_us)
{


	while (!_ende->getValue())
	{
		if(_wer->getValue()>0)
			_wer->SetValue(0);
		else
			_wer->SetValue(1);
		usleep(_t_us);
	}

	_wer->SetValue(0);





}



