/*
 * sinusoid.c
 *
 *  Created on: May 7, 2021
 *      Author: BG0FNDN
 */
#include "stm32f4xx_hal.h"
#include <math.h>

#include "sinusoid.h"
#define OFFSET 2047.5
#define ANALOG_MAX_VALUE 4095
#define NUMBER_POINTS 100 // 100 points total for T period of time
#define VCC 3300 // VCC = 3.3V => 3300mV
#define PI 3.14159265358979323846

int
voltToValueConverter (int mV)
{
  double step = (VCC * 1.0) / ANALOG_MAX_VALUE;

  return round(mV / step) - 1;
}

int
sinToAnalogValue (double val, int diapasonValue) // sin() => 0 ; ANALOG_MAX_VALUE
{
  return round(sin (val) * diapasonValue);
}

void
sinusoid (int mV, int T, DAC_HandleTypeDef *hdac)
{
  int t = round (T / NUMBER_POINTS); // t - the time for one sample

  int diapasonValue = voltToValueConverter (mV);

  if (diapasonValue >= ANALOG_MAX_VALUE)
    {
      return;
    }

  for (double i = 0; i < 2 * PI; i += 0.01)
    {
      HAL_DAC_SetValue (hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, round(sinToAnalogValue (i, diapasonValue) + OFFSET));
      HAL_Delay (t);
    }
}
