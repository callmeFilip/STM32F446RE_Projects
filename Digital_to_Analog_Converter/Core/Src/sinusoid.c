/*
 * sinusoid.c
 *
 *  Created on: May 7, 2021
 *      Author: BG0FNDN
 */
#include "stm32f4xx_hal.h"
#include <math.h>

#include "sinusoid.h"
#define OFFSET 2047
#define ANALOG_MAX_VALUE 4095
#define ANALOG_MIN_VALUE 0
#define VCC 3300 // VCC = 3.3V => 3300mV
#define SAMPLE_TIME 0.001
#define PI 3.1415926

int voltToDacValueConverter (double U)
{
  return (U * 1000) / ((VCC * 1.0) / ANALOG_MAX_VALUE);
}

void sinusoid (double U, int F, DAC_HandleTypeDef *hdac)
{
  int diapasonValue = voltToDacValueConverter (U);

  if (ANALOG_MAX_VALUE <= diapasonValue)
    {
      diapasonValue = ANALOG_MAX_VALUE;
    }
  else if (diapasonValue <= ANALOG_MIN_VALUE)
    {
      diapasonValue = ANALOG_MIN_VALUE;
    }

  double T = 1.0 / F;
  int N = T / SAMPLE_TIME;

  for (int n = 0; n < N; n++)
    {
      HAL_DAC_SetValue (hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R,
			voltToDacValueConverter(U * ( 1 + sin (2 * PI * n * SAMPLE_TIME / T) )));

      HAL_Delay (SAMPLE_TIME * 1000 - 1);
    }
}
