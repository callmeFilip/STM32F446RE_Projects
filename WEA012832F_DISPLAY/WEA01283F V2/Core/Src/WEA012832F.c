/*
 * WEA012832F.c
 *
 *  Created on: Jul 13, 2021
 *      Author: BG0FNDN
 */

#include "WEA012832F.h"
#include "stm32f4xx_hal.h"
#include <math.h>
#include <stdlib.h>

#define CTRL_BYTE_WRITE_DATA 0x40
#define CTRL_BYTE_WRITE_COMMAND 0x80

uint8_t Slave_Address = 0x78;
#define INIT_SEQ_LEN 23
#define COLUMNS_COUNT 128
#define ROWS_COUNT 4

#define ADDRESSING_MODE_HORIZONTAL 0x00
#define ADDRESSING_MODE_VERTICAL 0x01
#define ADDRESSING_MODE_PAGE_ADDRESSING 0x10

#define GRAPHIC_PAGE_LENGTH COLUMNS_COUNT * ROWS_COUNT + 1
#define GRAPHIC_PAGE_START_OFFSET 1

uint8_t tr_data[2] = { 0x00 };

uint8_t init_sequence[INIT_SEQ_LEN] = { 0xAE, // Display Off

    0xD5, // SET DISPLAY CLOCK
    0xF0,

    0xA8, // Select Miltiplex Ratio
    0x1F, //

    0xD3, // Setting Display Offset
    0x00, // 00H Reset, set common start

    0x40, // Set Display Start Line

    0x8D, // Set Charge Pump
    0x14, // Enable Charge Pump

    0xA1, // Set Segment Re-Map Default

    0xC8, // Set COM Output Scan Direction

    0xDA, // Set COM Hardware Configuration
    0x02, //

    0x81, // Set Contrast Control
    0xFF, //

    0xD9, // Set Pre-Charge period
    0x22, //

    0xDB, // Set Deselect Vcomh level
    0x40, //

    0xA4, // Entire Display ON

    0xA6, // Set Normal Display
    0xAF // Display ON
    };

uint8_t graphic_page[GRAPHIC_PAGE_LENGTH] = { CTRL_BYTE_WRITE_DATA };

uint8_t bitmap_hello[24] = { 0xFE, 0x10, 0x10, 0xFE, 0x00, 0xFE, 0x92, 0x92,
    0x92, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00, 0xFE, 0x02, 0x02, 0x02, 0x00,
    0x7C, 0x82, 0x82, 0x7C };

typedef struct Line
{
  int x, y;
} Line;

typedef struct Triangle
{
  Line point[3];
  int side[3];
} Triangle;

void write_command(FMPI2C_HandleTypeDef *hi2c, uint8_t data_byte)
{
  while(HAL_FMPI2C_GetState(hi2c) == HAL_FMPI2C_STATE_BUSY_TX)
    {
    }

  tr_data[0] = CTRL_BYTE_WRITE_COMMAND;
  tr_data[1] = data_byte;

  HAL_FMPI2C_Master_Transmit_DMA(hi2c, Slave_Address, tr_data, sizeof(tr_data));

}

void write_array_of_data(FMPI2C_HandleTypeDef *hi2c, uint8_t *array,
                         int length_in_bytes)
{
  while(HAL_FMPI2C_GetState(hi2c) != HAL_FMPI2C_STATE_READY);

  HAL_FMPI2C_Master_Transmit_DMA(hi2c, Slave_Address, array, length_in_bytes);
}

void set_page_address(FMPI2C_HandleTypeDef *hi2c, uint8_t page_start_address,
                      uint8_t page_end_address)
{
  write_command(hi2c, 0x22); // Set memory addressing mode
  write_command(hi2c, page_start_address);
  write_command(hi2c, page_end_address);
}

void write_to_graphic_page(uint8_t value, uint8_t x_input, uint8_t y_input)
{
  uint8_t y_com = floor(y_input / 8); // 8 stands for bits in byte
  uint8_t y_bit = y_input % 8; // 8 stands for bits in byte

  int byte_cell_index = (x_input * ROWS_COUNT + y_com)
      + GRAPHIC_PAGE_START_OFFSET;

  graphic_page[byte_cell_index] |= (value << y_bit);
}

void transmit_graphic_page(FMPI2C_HandleTypeDef *hi2c)
{
  write_array_of_data(hi2c, graphic_page, sizeof(graphic_page));
}

void clear_display(FMPI2C_HandleTypeDef *hi2c)
{
  for(int i = GRAPHIC_PAGE_START_OFFSET; i < GRAPHIC_PAGE_LENGTH; i++)
    {
      graphic_page[i] = 0x00; // empty segment
    }

  transmit_graphic_page(hi2c);
}

void set_page_addressing_mode(FMPI2C_HandleTypeDef *hi2c,
                              uint8_t addressing_mode)
{
  write_command(hi2c, 0x20); // Set memory addressing mode
  write_command(hi2c, addressing_mode);
}

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void draw_line(int x0, int y0, int x1, int y1)
{
  int steep = abs(y1 - y0) > abs(x1 - x0);

  // swap the co-ordinates if slope > 1 or we
  // draw backwards
  if(steep)
    {
      swap(&x0, &y0);
      swap(&x1, &y1);
    }
  if(x0 > x1)
    {
      swap(&x0, &x1);
      swap(&y0, &y1);
    }

  //compute the slope
  float dx = x1 - x0;
  float dy = y1 - y0;
  float gradient = dy / dx;
  if(dx == 0.0)
    gradient = 1;

  int xpxl1 = x0;
  int xpxl2 = x1;
  float intersectY = y0;

  // main loop
  if(steep)
    {
      int x;
      for(x = xpxl1; x <= xpxl2; x++)
        {
          // pixel coverage is determined by fractional
          // part of y co-ordinate
          write_to_graphic_page(0x01, round(intersectY), x);
          // write_to_graphic_page(0x01, round(intersectY)-1, x);
          intersectY += gradient;
        }
    }
  else
    {
      int x;
      for(x = xpxl1; x <= xpxl2; x++)
        {
          // pixel coverage is determined by fractional
          // part of y co-ordinate
          write_to_graphic_page(0x01, x, round(intersectY));
          // write_to_graphic_page(0x01, x, round(intersectY)-1);
          intersectY += gradient;
        }
    }

}

void draw_triangle(Triangle *triangle)
{
  draw_line(triangle->point[0].x, triangle->point[0].y, triangle->point[1].x,
            triangle->point[1].y);
  draw_line(triangle->point[0].x, triangle->point[0].y, triangle->point[2].x,
            triangle->point[2].y);
  draw_line(triangle->point[1].x, triangle->point[1].y, triangle->point[2].x,
            triangle->point[2].y);
}

void rotate_triangle(int cx, int cy, float angle, Triangle *triangle)
{
  float s = sinf(angle);
  float c = cosf(angle);

  for(int i = 0; i < 3; i++)
    {
      // translate point back to origin:
      triangle->point[i].x -= cx;
      triangle->point[i].y -= cy;

      // rotate point
      float xnew = triangle->point[i].x * c - triangle->point[i].y * s;
      float ynew = triangle->point[i].x * s + triangle->point[i].y * c;

      // translate point back:
      triangle->point[i].x = xnew + cx;
      triangle->point[i].y = ynew + cy;
    }
}

void fill_equaliteral_triangle_coordinates(int x0, int y0, int x1, int y1,
                                           Triangle *triangle)
{
  int side_length = x1 - x0;

  double height = side_length * sqrt(3.0) / 2;

  int x2 = ((x1 - x0) / 2) + x0;
  int y2 = y0 + height;

  triangle->point[0].x = x0;
  triangle->point[0].y = y0;

  triangle->point[1].x = x1;
  triangle->point[1].y = y1;

  triangle->point[2].x = x2;
  triangle->point[2].y = y2;

  draw_triangle(triangle);
}

void draw_sierpinski_fractal_private(FMPI2C_HandleTypeDef *hi2c, int x, int y,
                             double base, int height, int iterations)
{
  if(iterations <= 0)
    {
      return;
    }
  Triangle triangle = { 0 };

  triangle.point[0].x = x + (base / 2);
  triangle.point[0].y = y;

  triangle.point[1].x = x + (base / 4);
  triangle.point[1].y = y + height / 2;

  triangle.point[2].x = x + (base / 4) * 3;
  triangle.point[2].y = y + height / 2;

  draw_triangle(&triangle);

  iterations--;

  draw_sierpinski_fractal_private(hi2c, x, y, (base / 2), (height / 2), iterations);

  draw_sierpinski_fractal_private(hi2c, (x + (base / 4)), (y + height / 2), (base / 2),
                          (height / 2), iterations);

  draw_sierpinski_fractal_private(hi2c, x + (base / 2), y, (base / 2), (height / 2),
                          iterations);
}

void draw_sierpinski_fracrtal(FMPI2C_HandleTypeDef *hi2c, int base, int height, int x, int y, int iterations)
{
    Triangle tri = { 0 };

   double leg = sqrt(pow(height, 2) + pow((base / 2), 2));

   tri.side[0] = base;
   tri.side[1] = leg;
   tri.side[2] = leg;

   tri.point[0].x = x;
   tri.point[0].y = y;

   tri.point[1].x = x + base;
   tri.point[1].y = y;

   tri.point[2].x = tri.side[0] / 2 + tri.point[0].x;
   tri.point[2].y = y + height;

   draw_triangle(&tri);

   for(int i = 0 ; i <= iterations ; i++){
       draw_sierpinski_fractal_private(hi2c, x, y, base, height, i);
       transmit_graphic_page(hi2c);
       HAL_Delay(1000);
   }
}

void quick_animation(FMPI2C_HandleTypeDef *hi2c)
{
  Triangle init_triangle = { 0 };

  init_triangle.point[0].x = 50;
  init_triangle.point[0].y = 10;

  init_triangle.point[1].x = 80;
  init_triangle.point[1].y = 10;

  init_triangle.point[2].x = 65;
  init_triangle.point[2].y = 20;

  draw_triangle(&init_triangle);

  for(float i = 0 ; i < 2 ; i += 0.01)
    {
      rotate_triangle(65, 15, i, &init_triangle);
      draw_triangle(&init_triangle);
      transmit_graphic_page(hi2c);
//      HAL_Delay(10);
      clear_display(hi2c);
    }

  for(float i = 2.01 ; 0 < i ; i -= 0.01)
    {
      rotate_triangle(65, 15, i, &init_triangle);
      draw_triangle(&init_triangle);
      transmit_graphic_page(hi2c);
//      HAL_Delay(10);
      clear_display(hi2c);
    }


  clear_display(hi2c);
}

void D_Init(FMPI2C_HandleTypeDef *hi2c)
{
  for(int i = 0; i < INIT_SEQ_LEN; i++)
    {
      write_command(hi2c, init_sequence[i]);
    }

  set_page_addressing_mode(hi2c, ADDRESSING_MODE_VERTICAL);

  set_page_address(hi2c, 0, 3);

  clear_display(hi2c);

  for(int i = 0 ; i < 127 ; i++)
    {
      write_to_graphic_page(0x01, i+1, 0);
      write_to_graphic_page(0x00, i, 0);

      transmit_graphic_page(hi2c);
    }

  for(int i = 126 ; i >= 0 ; i--)
    {
      write_to_graphic_page(0x01, i, 31);
      write_to_graphic_page(0x00, i+1, 31);

      transmit_graphic_page(hi2c);
    }

  clear_display(hi2c);


  quick_animation(hi2c);

}
