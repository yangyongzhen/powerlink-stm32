/***************************************************************************//**
*  \file       main.c
*
*  \details    LED Blinking using RTX CMSIS V2 RTOS 
*
*  \author     EmbeTronicX
*
*  \Tested with Proteus
*
* *****************************************************************************/ 
#include <string.h>
#include <stdio.h>
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "stm32f10x.h"
 
#include "LED.h"
#include "Driver_USART.h"
#include "Driver_SPI.h"
#include "w5500_conf.h"

extern ARM_DRIVER_USART Driver_USART1;
extern ARM_DRIVER_SPI Driver_SPI2;
 
/* Variable definitions ------------------------------------------------------*/
static uint8_t rxBuffer[1024] = {0};
static uint8_t txBuffer[1024] = {0};
 
/* Function declarations -----------------------------------------------------*/
static void USART1_Callback(uint32_t event);
/**
  * @brief  USART1 callback function.
  * @param  event: USART events notification mask.
  * @return None.
  */
static void USART1_Callback(uint32_t event)
{
  if(event & ARM_USART_EVENT_RX_TIMEOUT)
  {
    Driver_USART1.Control(ARM_USART_ABORT_RECEIVE, 1);
    
    uint32_t length = Driver_USART1.GetRxCount();
    
    memcpy(txBuffer, rxBuffer, length);
    //printf("rx len:%d\n",length);
    //Driver_USART1.Send(txBuffer, length);
    Driver_USART1.Receive(rxBuffer, sizeof(rxBuffer));
  }
}

extern int stdout_init(void);
void uart_init()
{
	
	Driver_USART1.Initialize(USART1_Callback);
  Driver_USART1.PowerControl(ARM_POWER_FULL);
  Driver_USART1.Control(ARM_USART_MODE_ASYNCHRONOUS |
                        ARM_USART_DATA_BITS_8 |
                        ARM_USART_PARITY_NONE |
                        ARM_USART_STOP_BITS_1 |
                        ARM_USART_FLOW_CONTROL_NONE, 115200);
  Driver_USART1.Control(ARM_USART_CONTROL_TX, 1);
  Driver_USART1.Control(ARM_USART_CONTROL_RX, 1);
  
  Driver_USART1.Receive(rxBuffer, sizeof(rxBuffer));
}

void mySPI_callback(uint32_t event)
{
	
}
void spi_init()
{
	/* Initialize the SPI driver */
  Driver_SPI2.Initialize(mySPI_callback);
    /* Power up the SPI peripheral */
  Driver_SPI2.PowerControl(ARM_POWER_FULL);
    /* Configure the SPI to Master, 8-bit mode @10000 kBits/sec */
  Driver_SPI2.Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 10000000);
 
  /* SS line = INACTIVE = HIGH */
  Driver_SPI2.Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}

uint8_t spi_send_byte(uint8_t byte)
{
	uint8_t  dataout[2];
	uint8_t  datain[2];
	dataout[0] = byte;
	Driver_SPI2.Transfer(dataout,datain,1);
	return datain[0];
}
 
/*
** This thread will turns ON and turns OFF the PORT-D LEDs with 1second delay.
**
**  Arguments:
**      arg  -> Argument of this thread. osThreadNew()'s 2nd arg has to come here. 
**   
*/
__NO_RETURN static void LED_Blink_PortD( void *arg ) 
{
  (void)arg;                            //unused variable
  //set Port D as output
  //GPIOD->MODER = 0x55555555;
  for (;;)                              //infinite for loop
  {
    //Turn ON the LED of Port-D
    LED_On(0);
    osDelay(1000);                      //1sec delay
    //Turn OFF the LED of Port-D
    LED_Off(0);
    //GPIOD->BSRR = 0xFFFF0000;
    osDelay(1000);                      //1sec delay
  }
}
 
/*
** This thread will turns ON and turns OFF the PORT-E LEDs with 3second delay.
**
**  Arguments:
**      arg  -> Argument of this thread. osThreadNew()'s 2nd arg has to come here. 
**   
*/
__NO_RETURN static void LED_Blink_PortE( void *arg ) 
{
  (void)arg;                            //unused variable
  //set Port E as output
  //GPIOE->MODER = 0x55555555;
  for (;;)                              //infinite for loop
  {
    //Turn ON the LED of Port-E
    //GPIOE->BSRR = 0x0000FFFF;
		Driver_USART1.Send("123456", strlen("123456"));
		printf("hello test\r\n");
		set_w5500_ip();
	  //LED_SetOut(0);
		LED_On(1);
    osDelay(3000);  
		LED_Off(1);
		osDelay(3000);  
		//3sec delay
    //Turn OFF the LED of Port-E
    //GPIOE->BSRR = 0xFFFF0000;
    //osDelay(3000);                      //3sec delay
	  //LED_SetOut(1);
  }
}
 
/*
** main function
**
**  Arguments:
**      none
**   
*/ 
int main (void) 
{
  // System Initialization
  SystemCoreClockUpdate();
	
  LED_Initialize();
	uart_init();
	//stdout_init();
	printf("hello test\r\n");
	LED_On(2);
	spi_init();
	
	reset_w5500();
	set_w5500_mac();
	set_w5500_ip();
 
  osKernelInitialize();                       // Initialize CMSIS-RTOS
  osThreadNew(LED_Blink_PortD, NULL, NULL);   // Create application main thread
  osThreadNew(LED_Blink_PortE, NULL, NULL);   // Create application main thread
  osKernelStart();                            // Start thread execution
  for (;;) 
  {
    //Dummy infinite for loop.
  }
}