/*
 * SevenSegmentLedLibrary.h
 *
 *  Created on: Apr 22, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_SEVENSEGMENTLEDLIBRARY_H_
#define INC_SEVENSEGMENTLEDLIBRARY_H_
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define DISPLAYCOUNT 2
#define SEGMENTCOUNT 8
#define SYMBOLCOUNT 10 // symbols available

#define NUMBERSEGMENTS 8 // max segments used in number count
typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
} segment_t;

typedef struct
{
  segment_t segment[SEGMENTCOUNT];
} display_t;

typedef struct
{
  bool symbolMap[NUMBERSEGMENTS]; // boolean map of each number
} symbol_t;

void
configureSymbols (void);
void
displayDigit (const float number);
void
clearDisplay (const int displayIndex);
void
configureDisplays (void);

#endif /* INC_SEVENSEGMENTLEDLIBRARY_H_ */
