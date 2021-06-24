/*
 * change_GPIO.h
 *
 *  Created on: Jun 17, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_CHANGE_GPIO_H_
#define INC_CHANGE_GPIO_H_

#include "stm32f4xx_hal.h"

void set_GPIO_To_Input(GPIO_TypeDef *port, uint16_t i_pin);

void set_GPIO_To_Output(GPIO_TypeDef *port, uint16_t i_pin, uint32_t speed,
		uint32_t mode, uint32_t pull);

#endif /* INC_CHANGE_GPIO_H_ */
