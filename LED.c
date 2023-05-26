/*-----------------------------------------------------------------------------
 * Name:    LED_MCBSTM32C.c
 * Purpose: LED interface for MCBSTM32C evaluation board
 * Rev.:    1.0.0
 *----------------------------------------------------------------------------*/

/* Copyright (c) 2015 - 2017 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "LED.h"
#include "GPIO_STM32F10x.h"

const GPIO_PIN_ID Pin_LED[] = {
  { GPIOA,  8 },
  { GPIOD,  2 },
	{ GPIOA,  2 }, //W25Q64 CS
	{ GPIOC,  13 }, // CS
	{ GPIOC,  0 }, // reset
};

#define LED_COUNT (sizeof(Pin_LED)/sizeof(GPIO_PIN_ID))


/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Initialize (void) {
  uint32_t n;

  /* Configure pins: Push-pull Output Mode (50 MHz) with Pull-down resistors */
  for (n = 0; n < LED_COUNT; n++) {
    GPIO_PortClock   (Pin_LED[n].port, true);
    GPIO_PinWrite    (Pin_LED[n].port, Pin_LED[n].num, 0);
    GPIO_PinConfigure(Pin_LED[n].port, Pin_LED[n].num,
                      GPIO_OUT_PUSH_PULL,
                      GPIO_MODE_OUT2MHZ);
  }

  return 0;
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Uninitialize (void) {
  uint32_t n;

  /* Configure pins: Input mode, without Pull-up/down resistors */
  for (n = 0; n < LED_COUNT; n++) {
    GPIO_PinConfigure(Pin_LED[n].port, Pin_LED[n].num,
                      GPIO_IN_FLOATING,
                      GPIO_MODE_INPUT);
  }

  return 0;
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_On (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    GPIO_PinWrite(Pin_LED[num].port, Pin_LED[num].num, 1);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_Off (uint32_t num) {
  int32_t retCode = 0;

  if (num < LED_COUNT) {
    GPIO_PinWrite(Pin_LED[num].port, Pin_LED[num].num, 0);
  }
  else {
    retCode = -1;
  }

  return retCode;
}

/**
  \fn          int32_t LED_SetOut (uint32_t val)
  \brief       Write value to LEDs
  \param[in]   val  value to be displayed on LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t LED_SetOut (uint32_t val) {
  uint32_t n;

  for (n = 0; n < LED_COUNT; n++) {
    if (val & (1<<n)) {
      LED_On (n);
    } else {
      LED_Off(n);
    }
  }

  return 0;
}

/**
  \fn          uint32_t LED_GetCount (void)
  \brief       Get number of LEDs
  \return      Number of available LEDs
*/
uint32_t LED_GetCount (void) {

  return LED_COUNT;
}
