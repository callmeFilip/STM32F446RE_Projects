#include "SevenSegmentLedLibrary.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define DISPLAYCOUNT 2
#define SEGMENTCOUNT 8
#define NUMBERSEGMENTS 8 // max segments used in number count

#define SYMBOLCOUNT 10 // symbols available
#define DOTINDEX 7
#define DELAYTIME 20
#define MAX_BRIGHTNESS DELAYTIME
#define MIN_BRIGHTNESS 0

static int g_brightness = MAX_BRIGHTNESS;
static int g_darkness = MIN_BRIGHTNESS;

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pin;
} segment_t;

typedef struct
{
  segment_t segment[SEGMENTCOUNT];
} display_t;

typedef struct
{
  bool symbolMap[NUMBERSEGMENTS]; // boolean map of each number
} symbol_t;

display_t g_display[DISPLAYCOUNT];
symbol_t g_symbol[SYMBOLCOUNT];

void configureSymbols(void);
void configureDisplays(void);

void SSLL_Init(void)
{
  configureSymbols();
  configureDisplays();
}

/**
 * Configure all symbols
 */
void configureNumberZero()
{
  g_symbol[0].symbolMap[0] = true;
  g_symbol[0].symbolMap[1] = true;
  g_symbol[0].symbolMap[2] = true;
  g_symbol[0].symbolMap[3] = true;
  g_symbol[0].symbolMap[4] = true;
  g_symbol[0].symbolMap[5] = true;
  g_symbol[0].symbolMap[6] = false;
}

void configureNumberOne()
{
  g_symbol[1].symbolMap[0] = false;
  g_symbol[1].symbolMap[1] = true;
  g_symbol[1].symbolMap[2] = true;
  g_symbol[1].symbolMap[3] = false;
  g_symbol[1].symbolMap[5] = false;
  g_symbol[1].symbolMap[4] = false;
  g_symbol[1].symbolMap[6] = false;
}

void configureNumberTwo()
{
  g_symbol[2].symbolMap[0] = true;
  g_symbol[2].symbolMap[1] = true;
  g_symbol[2].symbolMap[2] = false;
  g_symbol[2].symbolMap[3] = true;
  g_symbol[2].symbolMap[4] = true;
  g_symbol[2].symbolMap[5] = false;
  g_symbol[2].symbolMap[6] = true;
}

void configureNumberThree()
{
  g_symbol[3].symbolMap[0] = true;
  g_symbol[3].symbolMap[1] = true;
  g_symbol[3].symbolMap[2] = true;
  g_symbol[3].symbolMap[3] = true;
  g_symbol[3].symbolMap[4] = false;
  g_symbol[3].symbolMap[5] = false;
  g_symbol[3].symbolMap[6] = true;
}

void configureNumberFour()
{
  g_symbol[4].symbolMap[0] = false;
  g_symbol[4].symbolMap[1] = true;
  g_symbol[4].symbolMap[2] = true;
  g_symbol[4].symbolMap[3] = false;
  g_symbol[4].symbolMap[4] = false;
  g_symbol[4].symbolMap[5] = true;
  g_symbol[4].symbolMap[6] = true;
}

void configureNumberFive()
{
  g_symbol[5].symbolMap[0] = true;
  g_symbol[5].symbolMap[1] = false;
  g_symbol[5].symbolMap[2] = true;
  g_symbol[5].symbolMap[3] = true;
  g_symbol[5].symbolMap[4] = false;
  g_symbol[5].symbolMap[5] = true;
  g_symbol[5].symbolMap[6] = true;
}

void configureNumberSix()
{
  g_symbol[6].symbolMap[0] = true;
  g_symbol[6].symbolMap[1] = false;
  g_symbol[6].symbolMap[2] = true;
  g_symbol[6].symbolMap[3] = true;
  g_symbol[6].symbolMap[4] = true;
  g_symbol[6].symbolMap[5] = true;
  g_symbol[6].symbolMap[6] = true;
}

void configureNumberSeven()
{
  g_symbol[7].symbolMap[0] = true;
  g_symbol[7].symbolMap[1] = true;
  g_symbol[7].symbolMap[2] = true;
  g_symbol[7].symbolMap[3] = false;
  g_symbol[7].symbolMap[4] = false;
  g_symbol[7].symbolMap[5] = false;
  g_symbol[7].symbolMap[6] = false;
}

void configureNumberEight()
{
  g_symbol[8].symbolMap[0] = true;
  g_symbol[8].symbolMap[1] = true;
  g_symbol[8].symbolMap[2] = true;
  g_symbol[8].symbolMap[3] = true;
  g_symbol[8].symbolMap[4] = true;
  g_symbol[8].symbolMap[5] = true;
  g_symbol[8].symbolMap[6] = true;
}

void configureNumberNine()
{
  g_symbol[9].symbolMap[0] = true;
  g_symbol[9].symbolMap[1] = true;
  g_symbol[9].symbolMap[2] = true;
  g_symbol[9].symbolMap[3] = true;
  g_symbol[9].symbolMap[4] = false;
  g_symbol[9].symbolMap[5] = true;
  g_symbol[9].symbolMap[6] = true;
}

void configureSymbols()
{
  configureNumberZero();
  configureNumberOne();
  configureNumberTwo();
  configureNumberThree();
  configureNumberFour();
  configureNumberFive();
  configureNumberSix();
  configureNumberSeven();
  configureNumberEight();
  configureNumberNine();

}

void dotOn(int displayIndex)
{
  HAL_GPIO_WritePin(g_display[displayIndex].segment[DOTINDEX].port,
		    g_display[displayIndex].segment[DOTINDEX].pin,
		    GPIO_PIN_RESET);
}

void dotOff(int displayIndex)
{
  HAL_GPIO_WritePin(g_display[displayIndex].segment[DOTINDEX].port,
		    g_display[displayIndex].segment[DOTINDEX].pin,
		    GPIO_PIN_SET);
}

void SSLL_clearAllDisplays()
{
  for (int displayIndex = 0; displayIndex < DISPLAYCOUNT; displayIndex++)
    {
      SSLL_clearDisplay(displayIndex);
    }
}

void SSLL_clearDisplay(const int displayIndex)
{
  for (int i = 0; i < NUMBERSEGMENTS; i++)
    {
      HAL_GPIO_WritePin(g_display[displayIndex].segment[i].port,
			g_display[displayIndex].segment[i].pin, GPIO_PIN_SET);
    }

  dotOff(displayIndex);
}

void configureDisplays(void)
{
  g_display[0].segment[0].port = GPIOB;
  g_display[0].segment[1].port = GPIOC;
  g_display[0].segment[2].port = GPIOA;
  g_display[0].segment[3].port = GPIOA;
  g_display[0].segment[4].port = GPIOA;
  g_display[0].segment[5].port = GPIOA;
  g_display[0].segment[6].port = GPIOB;
  g_display[0].segment[7].port = GPIOB;

  g_display[1].segment[0].port = GPIOC;
  g_display[1].segment[1].port = GPIOC;
  g_display[1].segment[2].port = GPIOC;
  g_display[1].segment[3].port = GPIOD;
  g_display[1].segment[4].port = GPIOC;
  g_display[1].segment[5].port = GPIOC;
  g_display[1].segment[6].port = GPIOA;
  g_display[1].segment[7].port = GPIOC;

  g_display[0].segment[0].pin = GPIO_PIN_9;
  g_display[0].segment[1].pin = GPIO_PIN_5;
  g_display[0].segment[2].pin = GPIO_PIN_12;
  g_display[0].segment[3].pin = GPIO_PIN_6;
  g_display[0].segment[4].pin = GPIO_PIN_11;
  g_display[0].segment[5].pin = GPIO_PIN_7;
  g_display[0].segment[6].pin = GPIO_PIN_12;
  g_display[0].segment[7].pin = GPIO_PIN_6;

  g_display[1].segment[0].pin = GPIO_PIN_10;
  g_display[1].segment[1].pin = GPIO_PIN_11;
  g_display[1].segment[2].pin = GPIO_PIN_12;
  g_display[1].segment[3].pin = GPIO_PIN_2;
  g_display[1].segment[4].pin = GPIO_PIN_9;
  g_display[1].segment[5].pin = GPIO_PIN_8;
  g_display[1].segment[6].pin = GPIO_PIN_15;
  g_display[1].segment[7].pin = GPIO_PIN_6;

}

void SSLL_changeBrightness(const int brightness)
{

  g_brightness = brightness;
  if (brightness < MIN_BRIGHTNESS)
    {
      g_brightness = MIN_BRIGHTNESS; // -1
    }
  else if (MAX_BRIGHTNESS < brightness)
    {
      g_brightness = MAX_BRIGHTNESS; // -1
    }

  g_darkness = DELAYTIME - g_brightness;

}

void displaySymbol(const int displayIndex, const int number)
{
  // iterate trough the symbol's map and set the diodes
  for (int i = 0; i < NUMBERSEGMENTS; i++)
    {
      if (g_symbol[number].symbolMap[i])
	{
	  HAL_GPIO_WritePin(g_display[displayIndex].segment[i].port,
			    g_display[displayIndex].segment[i].pin,
			    GPIO_PIN_RESET);
	}
      else
	{
	  HAL_GPIO_WritePin(g_display[displayIndex].segment[i].port,
			    g_display[displayIndex].segment[i].pin,
			    GPIO_PIN_SET);
	}
    }

}

void displayFloat(const int displayIndex, const float fnumber)
{
  int roundNumber = round(fnumber);

  dotOn(1); // display 1

  // iterate trough the symbol's map and set the diodes
  for (int i = 0; i < NUMBERSEGMENTS; i++)
    {
      if (g_symbol[roundNumber].symbolMap[i])
	{
	  HAL_GPIO_WritePin(g_display[displayIndex].segment[i].port,
			    g_display[displayIndex].segment[i].pin,
			    GPIO_PIN_RESET);
	}
    }
}

void run(const float number)
{
  // select display method
  if (number <= 99 && 10 <= number)
    {
      // extract first digit
      int firstDigit = round(number);

      while (firstDigit >= 10)
	{
	  firstDigit = firstDigit / 10;
	}

      // extract last digit
      int lastDigit = round(number);
      lastDigit %= 10;

      // display the digits
      displaySymbol(1, firstDigit);
      displaySymbol(0, lastDigit);
    }
  else if (number < 10 && 0 <= number)
    {
      SSLL_clearAllDisplays();

      // extract first digit
      int firstDigit = (int) number; // cuts the number after precision point

      // extract precision point
      int lastDigit = (int) (floor(number * 10)) % 10;

      while (firstDigit >= 10)
	{
	  firstDigit = firstDigit / 10;
	}

      // display the digits
      displayFloat(1, firstDigit);
      displayFloat(0, lastDigit);
    }
}

void SSLL_displayDigit(const float number)
{
  uint32_t tickstart = HAL_GetTick();

  while ((HAL_GetTick() - tickstart) < g_brightness)
    {
      run(number);
      HAL_Delay(0); // wait 1 ms
    }

  tickstart = HAL_GetTick();

  while ((HAL_GetTick() - tickstart) < g_darkness)
    {
      SSLL_clearAllDisplays();
      HAL_Delay(0); // wait 1 ms
    }

}

// Tests
void SSLL_testBrightness(void)
{
  for (int bright = 0; bright < 20; bright++)
    {
      SSLL_changeBrightness(bright);
      for (int i = 0; i < 4; i++)
	{
	  SSLL_displayDigit(88);
	}
    }

  for (int bright = 20; 0 <= bright; bright--)
    {
      SSLL_changeBrightness(bright);
      for (int i = 0; i < 4; i++)
	{
	  SSLL_displayDigit(88);
	}
    }
}

void SSLL_testDisplays(void)
{

  for (int i = 0; i < 100; i += 11)
    {
      dotOn(0);
      dotOn(1);
      SSLL_displayDigit(i);
      HAL_Delay(100);
    }

  SSLL_clearAllDisplays();
}
