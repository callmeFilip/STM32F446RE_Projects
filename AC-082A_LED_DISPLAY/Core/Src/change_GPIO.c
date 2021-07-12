/*
 * change_GPIO.c
 *
 *  Created on: Jun 17, 2021
 *      Author: BG0FNDN
 */

#include "change_GPIO.h"
#define PIN_COUNT 16
void set_GPIO_To_Input(GPIO_TypeDef *port, uint16_t i_pin) {
	uint32_t temp = 0x00U;

	uint32_t pin = 0x00U;

	/* Find Pin Position */
	for (int i = 0; i < PIN_COUNT; i++) {
		if (i_pin & (1 << i)) {
			pin = i;
			break;
		}
	}

	/* Configure the Pull-up or Pull down resistor to No Pull-up No Pull-Down state */
	temp = port->PUPDR;
	temp &= ~(3 << (pin * 2U));
	temp |= (GPIO_PULLDOWN << (pin * 2U));
	port->PUPDR = temp;

	/* Configure Input Direction mode */
	temp = port->MODER;
	temp &= ~(3 << (pin * 2U));
	port->MODER = temp;

}

void set_GPIO_To_Output(GPIO_TypeDef *port, uint16_t i_pin, uint32_t speed,
		uint32_t mode, uint32_t pull) {
	uint32_t temp = 0x00U;

	uint32_t pin = 0x00U;

	/* Find Pin Position*/
	for (int i = 0; i < PIN_COUNT; i++) {
		if (i_pin & (1 << i)) {
			pin = i;
			break;
		}
	}

	/* Set Speed */
	assert_param(IS_GPIO_SPEED(speed));

	temp = port->OSPEEDR;
	temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pin * 2U));
	temp |= (speed << (pin * 2U));
	port->OSPEEDR = temp;

	/* Configure the IO Output Type */
	temp = port->OTYPER;
	temp &= ~(GPIO_OTYPER_OT_0 << pin);
	temp |= (((mode & GPIO_OUTPUT_TYPE) >> 3U) << pin);
	port->OTYPER = temp;

	/* Configure the Pull-up or Pull down resistor */
	temp = port->PUPDR;
	temp &= ~(GPIO_PUPDR_PUPDR0 << (pin * 2U));
	temp |= (pull << (pin * 2U));
	port->PUPDR = temp;

	/* Configure Input Direction mode */
	temp = port->MODER;
	temp &= ~(3 << (pin * 2U));
	temp |= ((mode & GPIO_MODE) << (pin * 2U));
	port->MODER = temp;

}
