/*
 * pwm_module.h
 *
 *  Created on: Jul 7, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include "stm32f4xx_hal.h"

void pwm_setup(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t input_frequency,
               uint8_t duty_cycle_percentage);

#endif /* INC_PWM_H_ */
