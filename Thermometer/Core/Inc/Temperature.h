#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#include "stm32f4xx_hal.h"

float getTemperatureADCValue(ADC_HandleTypeDef * hadc, uint32_t timeout);

#endif
