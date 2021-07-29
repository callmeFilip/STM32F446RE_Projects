/*
 * WEA012832F.h
 *
 *  Created on: Jul 13, 2021
 *      Author: BG0FNDN
 */

#ifndef INC_WEA012832F_H_
#define INC_WEA012832F_H_
#include "stm32f4xx_hal.h"

void D_Init(FMPI2C_HandleTypeDef* hi2c);
void draw_sierpinski_fracrtal(FMPI2C_HandleTypeDef *hi2c, int base, int height, int x, int y, int iterations);
void clear_display(FMPI2C_HandleTypeDef *hi2c);

void quick_animation(FMPI2C_HandleTypeDef *hi2c);

#endif /* INC_WEA012832F_H_ */
