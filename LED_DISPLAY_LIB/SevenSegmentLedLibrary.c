#include "SevenSegmentLedLibrary.h"
#include "stm32f4xx_hal.h"

#include <math.h>

display_t g_display[DISPLAYCOUNT];
symbol_t g_symbol[SYMBOLCOUNT];

#define DOTINDEX 7

void
configureNumberZero ()
{
  g_symbol[0].symbolMap[0] = true;
  g_symbol[0].symbolMap[1] = true;
  g_symbol[0].symbolMap[2] = true;
  g_symbol[0].symbolMap[3] = true;
  g_symbol[0].symbolMap[4] = true;
  g_symbol[0].symbolMap[5] = true;

  g_symbol[0].symbolMap[6] = false;
}

void
configureNumberOne ()
{
  g_symbol[1].symbolMap[0] = false;
  g_symbol[1].symbolMap[1] = true;
  g_symbol[1].symbolMap[2] = true;
  g_symbol[1].symbolMap[3] = false;
  g_symbol[1].symbolMap[5] = false;
  g_symbol[1].symbolMap[4] = false;
  g_symbol[1].symbolMap[6] = false;
}

void
configureNumberTwo ()
{
  g_symbol[2].symbolMap[0] = true;
  g_symbol[2].symbolMap[1] = true;
  g_symbol[2].symbolMap[2] = false;
  g_symbol[2].symbolMap[3] = true;
  g_symbol[2].symbolMap[4] = true;
  g_symbol[2].symbolMap[5] = false;
  g_symbol[2].symbolMap[6] = true;
}

void
configureNumberThree ()
{
  g_symbol[3].symbolMap[0] = true;
  g_symbol[3].symbolMap[1] = true;
  g_symbol[3].symbolMap[2] = true;
  g_symbol[3].symbolMap[3] = true;
  g_symbol[3].symbolMap[4] = false;
  g_symbol[3].symbolMap[5] = false;
  g_symbol[3].symbolMap[6] = true;
}

void
configureNumberFour ()
{
  g_symbol[4].symbolMap[0] = false;
  g_symbol[4].symbolMap[1] = true;
  g_symbol[4].symbolMap[2] = true;
  g_symbol[4].symbolMap[3] = false;
  g_symbol[4].symbolMap[4] = false;
  g_symbol[4].symbolMap[5] = true;
  g_symbol[4].symbolMap[6] = true;
}

void
configureNumberFive ()
{
  g_symbol[5].symbolMap[0] = true;
  g_symbol[5].symbolMap[1] = false;
  g_symbol[5].symbolMap[2] = true;
  g_symbol[5].symbolMap[3] = true;
  g_symbol[5].symbolMap[4] = false;
  g_symbol[5].symbolMap[5] = true;
  g_symbol[5].symbolMap[6] = true;
}

void
configureNumberSix ()
{
  g_symbol[6].symbolMap[0] = true;
  g_symbol[6].symbolMap[1] = false;
  g_symbol[6].symbolMap[2] = true;
  g_symbol[6].symbolMap[3] = true;
  g_symbol[6].symbolMap[4] = true;
  g_symbol[6].symbolMap[5] = true;
  g_symbol[6].symbolMap[6] = true;
}

void
configureNumberSeven ()
{
  g_symbol[7].symbolMap[0] = true;
  g_symbol[7].symbolMap[1] = true;
  g_symbol[7].symbolMap[2] = true;
  g_symbol[7].symbolMap[3] = false;
  g_symbol[7].symbolMap[4] = false;
  g_symbol[7].symbolMap[5] = false;
  g_symbol[7].symbolMap[6] = false;
}

void
configureNumberEight ()
{
  g_symbol[8].symbolMap[0] = true;
  g_symbol[8].symbolMap[1] = true;
  g_symbol[8].symbolMap[2] = true;
  g_symbol[8].symbolMap[3] = true;
  g_symbol[8].symbolMap[4] = true;
  g_symbol[8].symbolMap[5] = true;
  g_symbol[8].symbolMap[6] = true;
}

void
configureNumberNine ()
{
  g_symbol[9].symbolMap[0] = true;
  g_symbol[9].symbolMap[1] = true;
  g_symbol[9].symbolMap[2] = true;
  g_symbol[9].symbolMap[3] = true;
  g_symbol[9].symbolMap[4] = false;
  g_symbol[9].symbolMap[5] = true;
  g_symbol[9].symbolMap[6] = true;
}

void
configureSymbols ()
{
  configureNumberZero ();
  configureNumberOne ();
  configureNumberTwo ();
  configureNumberThree ();
  configureNumberFour ();
  configureNumberFive ();
  configureNumberSix ();
  configureNumberSeven ();
  configureNumberEight ();
  configureNumberNine ();

}

void
dotOn (int displayIndex)
{
  HAL_GPIO_WritePin (g_display[displayIndex].segment[DOTINDEX].port,
                     g_display[displayIndex].segment[DOTINDEX].pin,
                     GPIO_PIN_RESET);
}

void
dotOff (int displayIndex)
{
  HAL_GPIO_WritePin (g_display[displayIndex].segment[DOTINDEX].port,
                     g_display[displayIndex].segment[DOTINDEX].pin,
                     GPIO_PIN_SET);
}

void
clearAllDisplays ()
{
  for (int displayIndex = 0; displayIndex < DISPLAYCOUNT; displayIndex++)
    {
      clearDisplay (displayIndex);
    }
}

void
clearDisplay (const int displayIndex)
{
  for (int i = 0; i < NUMBERSEGMENTS; i++)
    {
      HAL_GPIO_WritePin (g_display[displayIndex].segment[i].port,
                         g_display[displayIndex].segment[i].pin, GPIO_PIN_SET);
    }

  dotOff (displayIndex);
}

void
configureDisplays (void)
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

void
displayInteger (const int displayIndex, const int number)
{
  clearDisplay (displayIndex);
  for (int i = 0; i < NUMBERSEGMENTS; i++)
    {
      if (g_symbol[number].symbolMap[i])
        {
          HAL_GPIO_WritePin (g_display[displayIndex].segment[i].port,
                             g_display[displayIndex].segment[i].pin,
                             GPIO_PIN_RESET);
        }
    }

}

void
displayFloat (const int displayIndex, const float fnumber)
{
  int roundNumber = round (fnumber);
  clearDisplay (displayIndex);

  dotOn (1);

  for (int i = 0; i < NUMBERSEGMENTS; i++)
    {
      if (g_symbol[roundNumber].symbolMap[i])
        {
          HAL_GPIO_WritePin (g_display[displayIndex].segment[i].port,
                             g_display[displayIndex].segment[i].pin,
                             GPIO_PIN_RESET);
        }
    }
}

void
displayDigit (const float number)
{
  if (number <= 99 && 10 <= number)
    {
      clearAllDisplays ();
      int firstDigit = round (number);
      int lastDigit = round (number);
      lastDigit %= 10;

      while (firstDigit >= 10)
        {
          firstDigit = firstDigit / 10;
        }

      displayInteger (1, firstDigit);
      displayInteger (0, lastDigit);
    }
  else if (number < 10 && 0 <= number)
    {
      clearAllDisplays ();

      int firstDigit = (int) number; // cuts the number after precision point
      int lastDigit = (int) (floor (number * 10)) % 10;

      while (firstDigit >= 10)
        {
          firstDigit = firstDigit / 10;
        }

      displayFloat (1, firstDigit);
      displayFloat (0, lastDigit);
      firstDigit = firstDigit;
    }
}

