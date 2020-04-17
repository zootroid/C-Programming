/****************************************************************************
 *  Count_LCD_PTB.c
 *  Count from 1 to 100 on the LCD DMD using the 18F8722 on a 
 *   PIC TRAINER REV-1.28.
 * 
 *  Author:  Christopher Calvert
 *           <ccalvert1@ncstudents.niagaracollege.ca>
 *
 *  Important Note!
 *  ==============
 *  Both the pushbuttons and the SW1 DIP switches share PORTJ.  Make sure
 *   that ALL SW1 DIP switches are in the OFF position (1) BEFORE you run
 *   _any_ program that uses the matrix keypad.
 *
 ****************************************************************************
 *  Originally written by Professor Mark Csele 2006/09
 *  as LCD Test program for the 18F452 on a PICPROTO-II (up to 20MHz)
 ****************************************************************************
 *  Modification History for LCDTest.c:
 *  ----------------------------------
 *  2011.02.14  Merged with template for CTEC1904/2011W
 *               to update CONFIG directives, etc.
 *  2011.02.15  Modified to work with Lab #3 counter application.
 *              Converted to 8-bit mode.
 *  2013.01.07  Successfully tested on PIC TRAINER REV-1.28
 *  2013.01.16  (MB) Created template from Buttons.asm (Pushbuttons_CtoF_PTB)
 *  2013.01.27  (MB) Changed oscillator setting from XT to HS; added tables
 *  2014.01.15  (MB) Updated for 2014W
 *  2014.03.12  (MB) Added possible hardware configs and libraries
 *  2019.01.14  (MB) Translated to C and ported to MPLAB X
 *  2019.01.19  (MB) Ported LCDTest18.asm and LCD.asm to C
 *  2019.01.19  (MB) Use printf() with LCD definition of putch()
 ****************************************************************************
 *  Modification History:
 *  --------------------
 *  2013.01.16  (MB) Created template from Buttons.asm (Pushbuttons_CtoF_PTB)
 *  2013.01.27  (MB) Changed oscillator setting from XT to HS; added tables
 *  2014.01.15  (MB) Updated for 2014W
 *  2014.03.12  (MB) Added possible hardware configs and libraries
 *  2019.01.14  (MB) Translated to C and ported to MPLAB X
 *  2019.01.18  (MB) Ported from KeypadCtoF.asm (Lab 2, Program 13, 2018W)
 *  2019.01.19  (MB) Combined Lab43_PTB.c, Keypad.asm (Lab3, Program 15, 
 *		      2018W) and Lab34_PTB.c (for 7-seg. codes)
 *  2019.01.21  (MB) Implement the standard C getch() function.
 *  2019.02.12  (MB) Merged with LCDTest.c to combine getch() with putch()
 *  2019.02.24  (MB) This is now CTEC1904/2019W Lab 6.2
 *  2019.03.08  (MB) Copied from Lab 6.2 and transformed into Lab 7.1,
 *                    with config_PTB.h taken from Lab 5.1
 *  2019.03.10  (MB) Converted to template for Lab 7
 *  2020.02.24  (MB) Updated for 2020W (This is now Lab 6)
 *
 *  2020.04.16  (CC) Started Lab 6.1
 *  2020.04.16  (CC) Finished Lab 6.1 complete with comments.
 *
 ****************************************************************************
 *  MPLAB X Version: 	5.20
 *  XC8 Version:        2.05
 *  Debugger:           PICkit3
 *
 *        1         2         3         4         5         6         7
 *23456789012345678901234567890123456789012345678901234567890123456789012345678
 *
 ****************************************************************************
 *  Hardware:
 *  --------
 *  PIC18F8722 MCU
 *  CLK = 4.9152 MHz (f_osc)
 *
 *  RH0..RH7	=>	LCD DATA 0..7 (handled by LCD.c)
 *  RE0         =>	LCD RS
 *  RE1         =>	LCD E
 *  RE2         =>	LCD R/nW
 *
 *  ADD YOUR HARDWARE COMMENTS HERE FOR THE MATRIX KEYPAD
 *
 *  Board Configuration DIP Switch Settings
 *  =======================================
 *  SW2-1..SW2-4	x
 *  SW3-1 (KEYSC-F)	RB5|GND
 *  SW3-2..SW3-4	x
 *
 *  IMPORTANT NOTE!!!
 *  --------------
 *  PICTRAINER BOARD uses SHARED I/O -- PORT J is shared between SW1
 *   DIP switches, which are wired ACTIVE LOW with 10K pull-up resistors.
 * 
 *  -> Attempting to assert rows active HIGH with DIP switch(es) closed
 *      will short PORT B pins to ground, while reading constant LOW on
 *      PORT J.
 *
 ****************************************************************************/

#include <xc.h>
#include <stdio.h>	/* for printf() and puts() */

/*** Programming Configuration Information ***/
#include "config_PTB.h"
/*** Programming Configuration ENDS ***/

#include "LCD.h"


/****************************************************************************
 * Constant Defines
 ***************************************************************************/

#define START_COUNT     8	/* Start COUNT at 8 */
#define END_COUNT       100	/* Finish COUNT at 100 */
#define DELAY_TIME_MS	273	/* Delay TIME 273ms */


/****************************************************************************
 * Subroutines
 ****************************************************************************/

void delay (void);                  // Initialize delay function
void waitForButtonPress (void);     // Initialize wait for button to be pressed
                                    // function.


/****************************************************************************
 * Global Variables
 ***************************************************************************/

/* (none) */


/****************************************************************************
 * Main Program Begins
 ***************************************************************************/

int
main( void )
{
	/* ADD A LOCAL VARIABLE HERE TO BE THE COUNTER */
    int count;                              // Variable used to display on the 
                                            // LCD
    
	
	ADCON1 = 0x0f ;        /* All analog I/O pins are digital */

    
	/* CONFIGURE I/O PORTS_HERE */

	LCD_Init();                             // Initialize LCD

    /*Initialize Matrix Keypad*/
    
	LATB = 0xff ;           /* All PORT B pins are OFF (active low) */
	TRISB = 0b11000011 ;	/* RB2..RB5 are outputs; RB0 & RB1 are unused;
            				   RB6 & RB7 are dedicated to ICD */
	
	TRISJ = 0xff ;          /* RJ0..RJ3 are inputs; RJ4..RJ7 are unused */
		
    
    
	/* MAIN PROGRAM GOES HERE */

    waitForButtonPress();                   // Keeps LCD blank until a button is
                                            // pressed to start the count 
                                            // sequence.
	
	
    for (count = START_COUNT; count <= END_COUNT; count++)
    {
       LCD_Char(printf("%3d", count));      // Sends the current count to the 
                                            // LCD.
       
       delay();                             // Call delay
       
       LCD_Home();                          // Set cursor to initial position.
                                            // Must have because of 
                                            // auto-increment.
    }
       
       LCD_Line2();                         // Change to line 2
       
       LCD_Char(printf("DONE"));            // Displays 'DONE' on LCD

	while ( 1 ) ;			  /* Halt */
		
	return 0 ;
    }


/****************************************************************************
 * Subroutines
 ****************************************************************************/

/**************************************************************************
 *  Function:	  delay     
 *  Description:  Generic millisecond-range delay
 *  Parameters:   none	
 *  Returns:      none
 *  Notes:        Dependent on _XTAL_FREQ (must be defined for _this_ 
 *                 board)
 *                
 **************************************************************************/
void 
delay(void)
{
	__delay_ms(273);                        // Delay time of 273ms
}


/**************************************************************************
 *  Function:	  Waits for button to be pressed     
 *  Description:  Loops endlessly until you press #8 on the keypad.
 *  Parameters:   none	
 *  Returns:      none
 *  Notes:        Uses RB4 and RJ0 to break out of loop
 *                
 **************************************************************************/
void waitForButtonPress (void)
{
    while (1)     
    {
        ;                                   // Do nothing.  Just endlessly loop.
        PORTBbits.RB4 = 0;                  // Turns on RB4 (active low)
        if (PORTJbits.RJ0 == 0)             // When RJ0 is 0, break out of while
                                            // statement.
        {
            break;                          // Breaks from while loop.
        }
    }
}

/*** END ***/
