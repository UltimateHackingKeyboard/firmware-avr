#include "PCA9634.h"

void PCA9634_Init()
{
  // Enable internal oscillator.
  TWI_Start();
  TWI_Write(PCA9634_ADDRESS);
  TWI_Write(PCA9634_REG_MODE1);
  TWI_Write(0b00001);
  TWI_Stop();

  TWI_Start();
  TWI_Write(PCA9634_ADDRESS);
  TWI_Write(PCA9634_AI_ALL | PCA9634_REG_LEDOUT0);
  TWI_Write(0b10101010);  // Enable PWM mode for PWM0 to PWM3.
  TWI_Write(0b10101010);  // Enable PWM mode for PWM4 to PWM7.
  TWI_Stop();
}

