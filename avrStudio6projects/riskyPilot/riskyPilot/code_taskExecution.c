/*
 * Femto OS v 0.92 - Copyright (C) 2008-2010 Ruud Vlaming
 *
 * This file is part of the Femto OS distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Please note that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */


/* This this the only include needed in your code .*/
#include "femtoos_code.h"
#include "globals.h"

//Tuint08 corresponds to data type uint8_t
//-> re-defintion in file "femtoos_globals.h"
//static Tuint08 speed = 0;

//make sure the string is put into flash
//const Tchar HelloWorldStr[] PROGMEM = "Hello World.";



//function is called at system start-up
//-> initialisation can be done here
void appBoot(void)
{ 
	myInit();
    
    //cli();
	uart_init(UART_BAUD_SELECT(9600,F_CPU));	
	//sei();
    
	//pwmInit(100);
    //sei();   
    
    /*
    while (1)
    {
        uart_puts("hello\r\n");
        //uart_putc('h');
        _delay_ms(10);
        
    }
	*/
	//while(1)
    //{}

}



#if (preTaskDefined(Communication))

void appLoop_Communication(void)
{ 
	volatile uint8_t c;

	while (true)
	{ 	
		PORTA ^= (1<<PINA7);
        
        sei();
        uart_putc('x');
        
		//uart_puts("hello, damn it \r\n");        
		
      
        
        taskDelayFromNow(1000); 
        

        
	} 
}

#endif



#if (preTaskDefined(EvaluateRc))

void appLoop_EvaluateRc(void)
{
	//#warning "what the fuck!"
	
	while (true)
	{ 



			
		taskDelayFromNow(1000); 
	}
	
}

#endif

