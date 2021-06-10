/*
 * AC-082A_LIB.h
 *
 *  Created on: Jun 10, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_AC_082A_LIB_H_
#define INC_AC_082A_LIB_H_

#include "stm32f4xx_hal.h"

void AC082A_Init(GPIO_PinState line_mode, GPIO_PinState display_font,
		 GPIO_PinState cursor, GPIO_PinState blink,
		 GPIO_PinState incr_decr_mode, GPIO_PinState entire_shift);

void AC082A_Clear(void);

/*
 * Set cursor moving and display shift control
 * bit, and the direction, without changing of
 * DDRAM data.
 */
void AC082A_Cursor_Or_Display_Shift(GPIO_PinState SC, GPIO_PinState RL);

/*
 * Sets interface data length (DL: 8-bit / 4-bit),
 * number of display lines (N: 2-line/1-line) and,
 * display font type (F: 5x11 dots / 5x8 dots).
 */
void AC082A_Function_Set(GPIO_PinState length_mode, GPIO_PinState line_mode,
			 GPIO_PinState display_font);

/*
 * Set display (D), cursor (C), and blink of
 * cursor (B) on/off control bit
 */
void AC082A_Display_On_Off_Control(GPIO_PinState display, GPIO_PinState cursor,
				   GPIO_PinState blink);

/*
 * Assign cursor moving direction and
 * enable the shift of entire display.
 */
void AC082A_Entry_Mode_Set(GPIO_PinState incr_decr_mode, GPIO_PinState entire_shift);

/*
 * Sets DD RAM address to “00H” from AC
 * and return cursor to its original position if
 * shifted. The contents of DDRAM are not changed.
 */
void AC082A_Return_Home(void);

void AC082A_Write_Text(char * text);
#endif /* INC_AC_082A_LIB_H_ */
