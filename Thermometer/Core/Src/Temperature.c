/*
 * Temperature.c
 *
 *  Created on: May 20, 2021
 *      Author: BG0FNDN
 */

#include "Temperature.h"
#include <string.h>

#define MAX_ADC_VALUE 4095
#define MIN_ADC_VALUE 0

#define MAX_T 125.0

#define BUFFSIZE 1000

float ADC_Value_To_Degree (int ADC_Value)
{
  if (MAX_ADC_VALUE < ADC_Value)
    {
      ADC_Value = MAX_ADC_VALUE;
    }
  else if (ADC_Value < MIN_ADC_VALUE)
    {
      ADC_Value = MIN_ADC_VALUE;
    }

  return ((MAX_T / MAX_ADC_VALUE) * ADC_Value) - 10;
}

float getTemperatureADCValue (ADC_HandleTypeDef *hadc, uint32_t timeout)
{
  float temperature = 0;

  for (int i = 0; i < BUFFSIZE; i++)
    {
      temperature += ADC_Value_To_Degree (HAL_ADC_GetValue (hadc));
    }

  return temperature / BUFFSIZE;
}
