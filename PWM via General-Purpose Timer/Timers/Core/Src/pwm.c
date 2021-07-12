#include "pwm.h"
#include <math.h>

#define MAX_UINT32 4294967295
#define PROCESSOR_FREQUENCY 84000000 // Hz

void pwm_setup(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t input_frequency,
               uint8_t duty_cycle_percentage)
{
  uint32_t counting_period = 0;
  uint32_t prescaler = 0;

  uint32_t Prescaler_and_CountingPeriod = round(
  PROCESSOR_FREQUENCY / input_frequency);

  if(Prescaler_and_CountingPeriod < MAX_UINT32)
    {
      counting_period = Prescaler_and_CountingPeriod - 1;
      prescaler = 0;
    }
  else
    {
      counting_period = round(Prescaler_and_CountingPeriod / 100) - 1;
      prescaler = 99;
    }

  uint32_t pulse = (counting_period / 100) * duty_cycle_percentage;

  htim->Instance->ARR = counting_period; // Set Counting Period

  htim->Instance->PSC = prescaler; // Set Prescaler

  // Set Pulse
  switch(Channel)
  {
    case TIM_CHANNEL_1:
      htim->Instance->CCR1 = pulse;
      break;
    case TIM_CHANNEL_2:
      htim->Instance->CCR2 = pulse;
      break;
    case TIM_CHANNEL_3:
      htim->Instance->CCR3 = pulse;
      break;
    case TIM_CHANNEL_4:
      htim->Instance->CCR4 = pulse;
      break;
    default:
      break;
  }

  /* Generate an update event to reload the Prescaler
     and the repetition counter (only for advanced timer) value immediately */
  htim->Instance->EGR = TIM_EGR_UG;
}
