/*
 * AC-082A_LIB.c
 *
 *  Created on: Jun 10, 2021
 *      Author: BG0FNDN
 */

#include "AC-082A_Lib.h"

#include <string.h>

#define LINE_LENGTH 8
#define TOTAL_LENGTH 48
#define SECOND_LINE_START 40

#define E(PSTATE) \
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, PSTATE) // E

#define RS(PSTATE) \
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, PSTATE) // RS

#define RW(PSTATE) \
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, PSTATE) // R/W

#define DB7(PSTATE) \
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, PSTATE) // DB7

#define DB6(PSTATE) \
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, PSTATE) // DB6

#define DB5(PSTATE) \
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, PSTATE) // DB5

#define DB4(PSTATE) \
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, PSTATE) // DB4

#define DB3(PSTATE) \
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, PSTATE) // DB3

#define DB2(PSTATE) \
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, PSTATE) // DB2

#define DB1(PSTATE) \
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, PSTATE) // DB1

#define DB0(PSTATE) \
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, PSTATE) // DB0

void AC082A_Init (GPIO_PinState line_mode, GPIO_PinState display_font,
                  GPIO_PinState cursor, GPIO_PinState blink,
                  GPIO_PinState incr_decr_mode, GPIO_PinState entire_shift)
{
    // Wait for more than 30ms after VDD rises to 4.5V
    HAL_Delay (31); // 31 ms

    // Function Set
    AC082A_Function_Set (GPIO_PIN_SET, line_mode, display_font);

    // Wait for more than 39 microseconds
    HAL_Delay (1); // 1ms

    // Display ON/OFF Control
    AC082A_Display_On_Off_Control (display_font, cursor, blink);

    // Wait for more than 39 microseconds
    HAL_Delay (1); // 1ms

    // Display Clear
    AC082A_Clear ();

    // Wait for more than 1.53 ms
    HAL_Delay (2); // 2ms

    // Entry Mode Set
    AC082A_Entry_Mode_Set (incr_decr_mode, entire_shift);

    HAL_Delay (0); // 1ms
} // ~ Initialization end

void AC082A_Display_On_Off_Control (GPIO_PinState display, GPIO_PinState cursor,
                                    GPIO_PinState blink)
{
    RS(GPIO_PIN_RESET); // 0
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E

    DB7(GPIO_PIN_RESET); // 0
    DB6(GPIO_PIN_RESET); // 0
    DB5(GPIO_PIN_RESET); // 0
    DB4(GPIO_PIN_RESET); // 0
    DB3(GPIO_PIN_SET); // 1
    DB2(display); // D
    DB1(cursor); // C
    DB0(blink); // B

    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (0); // 1ms

}

void AC082A_Entry_Mode_Set (GPIO_PinState incr_decr_mode,
                            GPIO_PinState entire_shift)
{
    RS(GPIO_PIN_RESET); // 0
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E

    DB7(GPIO_PIN_RESET); // 0
    DB6(GPIO_PIN_RESET); // 0
    DB5(GPIO_PIN_RESET); // 0
    DB4(GPIO_PIN_RESET); // 0
    DB3(GPIO_PIN_RESET); // 0
    DB2(GPIO_PIN_SET); // 1
    DB1(incr_decr_mode); // I/D
    DB0(entire_shift); // SH

    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (0); // 1ms

}

void AC082A_Function_Set (GPIO_PinState length_mode, GPIO_PinState line_mode,
                          GPIO_PinState display_font)
{
    RS(GPIO_PIN_RESET); // 0
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E

    DB7(GPIO_PIN_RESET); // 0
    DB6(GPIO_PIN_RESET); // 0
    DB5(GPIO_PIN_SET); // 1
    DB4(length_mode); // DL
    DB3(line_mode); // N
    DB2(display_font); // F
    DB1(GPIO_PIN_RESET); // x
    DB0(GPIO_PIN_RESET); // x

    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (0); // 1ms

}

void AC082A_Cursor_Or_Display_Shift (GPIO_PinState SC, GPIO_PinState RL)
{
    RS(GPIO_PIN_RESET); // 0
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E
    HAL_Delay (0); // 1ms

    DB7(GPIO_PIN_RESET); // 0
    DB6(GPIO_PIN_RESET); // 0
    DB5(GPIO_PIN_RESET); // 0
    DB4(GPIO_PIN_SET); // 0
    DB3(SC); // S/C
    DB2(RL); // R/L
    DB1(GPIO_PIN_RESET); // x
    DB0(GPIO_PIN_RESET); // x

    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (0); // 1ms

}

void AC082A_Clear (void)
{
    RS(GPIO_PIN_RESET); // 0
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E

    DB7(GPIO_PIN_RESET); // 0
    DB6(GPIO_PIN_RESET); // 0
    DB5(GPIO_PIN_RESET); // 0
    DB4(GPIO_PIN_RESET); // 0
    DB3(GPIO_PIN_RESET); // 0
    DB2(GPIO_PIN_RESET); // 0
    DB1(GPIO_PIN_RESET); // 0
    DB0(GPIO_PIN_SET); // 1

    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (1); // 2ms
}

void AC082A_Return_Home (void)
{
    RS(GPIO_PIN_RESET); // 0
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E

    DB7(GPIO_PIN_RESET); // 0
    DB6(GPIO_PIN_RESET); // 0
    DB5(GPIO_PIN_RESET); // 0
    DB4(GPIO_PIN_RESET); // 0
    DB3(GPIO_PIN_RESET); // 0
    DB2(GPIO_PIN_RESET); // 0
    DB1(GPIO_PIN_SET); // 1
    DB0(GPIO_PIN_RESET); // x
    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (1); // 2ms
}

void AC082A_Write_Char (char ch)
{
    RS(GPIO_PIN_SET); // 1
    RW(GPIO_PIN_RESET); // 0

    E(GPIO_PIN_SET); // E

    DB7(ch & 128); // D7
    DB6(ch & 64); // D6
    DB5(ch & 32); // D5
    DB4(ch & 16); // D4
    DB3(ch & 8); // D3
    DB2(ch & 4); // D2
    DB1(ch & 2); // D1
    DB0(ch & 1); // D0

    HAL_Delay (0); // 1ms

    E(GPIO_PIN_RESET); // E

    HAL_Delay (0);
}

void AC082A_Write_Text (char *ch)
{
    int char_iterator = 0;
    int txt_size = strlen (ch);

    if (txt_size < LINE_LENGTH)
    {
        for (int i = 0; i < txt_size; i++)
        {
            AC082A_Write_Char (ch[i]);
        }
    }
    else if (txt_size < (2 * LINE_LENGTH))
    {
        for (int i = 0; i < txt_size; i++)
        {
            AC082A_Write_Char (ch[i]);
        }

        for (int i = LINE_LENGTH; i < SECOND_LINE_START; i++)
        {
            AC082A_Cursor_Or_Display_Shift (GPIO_PIN_SET, GPIO_PIN_SET);
        }

        for (int i = LINE_LENGTH; i < txt_size; i++)
        {
            AC082A_Write_Char (ch[i]);
        }

    }
    else
    {
    }
}
