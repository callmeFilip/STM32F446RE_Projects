/*
 * SevenSegmentLedLibrary.h
 *
 *  Created on: Apr 22, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_SEVENSEGMENTLEDLIBRARY_H_
#define INC_SEVENSEGMENTLEDLIBRARY_H_

#include "stm32f4xx_hal.h"

void SSLL_Init (void);
void displayDigit (const float number);
void clearDisplay (const int displayIndex);

#endif /* INC_SEVENSEGMENTLEDLIBRARY_H_ */
