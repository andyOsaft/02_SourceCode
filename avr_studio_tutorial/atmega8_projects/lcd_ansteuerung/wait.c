//--------------------------------------------------------------------------------------------------------------
//	wait_ms(...) - Wartet einige Millisekunden
//	PE:	milliSec=Anzahl der zu wartenden Millisekunden
//--------------------------------------------------------------------------------------------------------------

void	wait_ms(int milliSec)
{
	_delay_loop_2(1*(F_CPU/(1000/4))*milliSec); 	// 4 Zyklen Warteschleife
}

void 	wait_us(int mikroSec)
{
	_delay_loop_2(1*(F_CPU/(1000000/4)) * mikroSec);	// 4 Zyklen Warteschleife
}

//--------------------------------------------------------------------------------------------------------------
