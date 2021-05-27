/*
 * Temperature.c
 *
 *  Created on: May 20, 2021
 *      Author: BG0FNDN
 */

#include "Temperature.h"
#include <string.h>
#include <stdbool.h>

#define MAX_ADC_VALUE 4095
#define MIN_ADC_VALUE 0
#define MAX_T 125.0
#define ARRSIZE 100

static bool filled = false;

static float arr[ARRSIZE] =
  { 0.0f };
static int iter = 0;

float findAvg (const float *arr)
{
  float result = 0;

  for (int i = 0; i < ARRSIZE; i++)
    {
      result += arr[i];
    }

  return result / ARRSIZE;
}

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
  if (iter < ARRSIZE)
    {
      iter++;
    }
  else
    {
      iter = 0;
      filled = true;
    }

  if (HAL_ADC_PollForConversion (hadc, timeout) == HAL_OK)
    {
      arr[iter] = ADC_Value_To_Degree (HAL_ADC_GetValue (hadc));
    }

  if (filled)
    {
      return findAvg (arr);
    }
  else
    {
      return 0;
    }
}
