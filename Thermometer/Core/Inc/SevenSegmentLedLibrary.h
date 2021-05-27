/*
 * SevenSegmentLedLibrary.h
 *
 *  Created on: Apr 22, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_SEVENSEGMENTLEDLIBRARY_H_
#define INC_SEVENSEGMENTLEDLIBRARY_H_

#include "stm32f4xx_hal.h"

/**
 * void SSLL_Init(void);
 * Initialize the displays and symbols.
 */
void SSLL_Init(void);

/**
 * void displayDigit(const float number);
 * Displays digit to the given display.
 * Select display via parameter.
 */
void SSLL_displayDigit(const float number);

/**
 * void changeBrightness(const int brightnessLevel);
 * Changes the brightness of the displays.
 * Has at least 10ms delay.
 * Select brightness level via parameter.
 */
void SSLL_changeBrightness(const int brightnessLevel);

/**
 * void clearDisplay(const float number);
 * Clears the given display.
 * Select display via parameter.
 */
void SSLL_clearDisplay(const int displayIndex);

/**
 * void SSLL_clearAllDisplays(void);
 * Clears all displays.
 */
void SSLL_clearAllDisplays(void);

/**
 * void SSLL_testBrightness(void);
 * Tests the brightness while showing the number 88.
 */
void SSLL_testBrightness(void);

/**
 * void SSLL_testDisplays(void);
 * Tests the displays showing the numbers from 0 to 99.
 */
void SSLL_testDisplays(void);

#endif /* INC_SEVENSEGMENTLEDLIBRARY_H_ */
