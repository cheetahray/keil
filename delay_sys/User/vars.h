#include <stm32f10x.h>
#include <stdbool.h>

#define GPIO_BROWN_2_0              GPIO_Pin_0
#define GPIO_ORANGE_2_1             GPIO_Pin_1
#define GPIO_YELLOW_2_2             GPIO_Pin_2
#define GPIO_GREEN_2_3              GPIO_Pin_3
#define GPIO_BLUE_2_4               GPIO_Pin_4
#define GPIO_BLUEWHITE_2_5          GPIO_Pin_5
#define GPIO_GRAY_2_6               GPIO_Pin_6
#define GPIO_GRAYWHITE_2_7          GPIO_Pin_7
#define GPIO_BROWNWHITE_DIRECTION   GPIO_Pin_8
#define ROTARY_ENCODER_ANGLE        GPIOA

#define _2_0_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_BROWN_2_0)/* GPIOB->IDR  & GPIO_Pin_6   */
#define _2_1_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_ORANGE_2_1)/*GPIOB->IDR  & GPIO_Pin_7	  */
#define _2_2_read       	GPIO_ReadInputDataBit(GPIOB,                GPIO_YELLOW_2_2)/* GPIOB->IDR  & GPIO_Pin_6   */
#define _2_3_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_GREEN_2_3)/*GPIOB->IDR  & GPIO_Pin_7	  */
#define _2_4_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_BLUE_2_4)/* GPIOB->IDR  & GPIO_Pin_6   */
#define _2_5_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_BLUEWHITE_2_5)/*GPIOB->IDR  & GPIO_Pin_7	  */
#define _2_6_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_GRAY_2_6)/* GPIOB->IDR  & GPIO_Pin_6   */
#define _2_7_read       	GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_GRAYWHITE_2_7)/*GPIOB->IDR  & GPIO_Pin_7	  */
//#define direction_read      GPIO_ReadInputDataBit(ROTARY_ENCODER_ANGLE, GPIO_BROWNWHITE_DIRECTION)

#define GPIO_PWM_PIN                GPIO_Pin_2
#define GPIO_USART_TX_PIN           GPIO_Pin_9
#define GPIO_USART_RX_PIN           GPIO_Pin_10
#define GPIO_ADC_PIN                GPIO_Pin_0

#define PWM_TIM_PERIOD              29                              // Number of CPU cycles that will constitute 1 period
#define PWM_HIGH_WIDTH              17                              // Duty cycle of pwm signal for a logical 1 to be read by the ws2812 chip. Duty cycle = PWM_HIGH_WIDTH/TIM_PERIOD*100
#define PWM_LOW_WIDTH               9                               // Duty cycle of pwm signal for a logical 0 to be read by the ws2812 chip. Duty cycle = PWM_LOW_WIDTH/TIM_PERIOD*100

#define COLUMBS                     120
#define ROWS                        2

/* Buffer that holds one complete DMA transmission.
 *
 * The buffer size can be calculated as followas:
 * number of LEDs * 24 bytes + 42 bytes.
 *
 * This leaves us with a maximum string length of
 * (2^16 bytes per DMA stream - 42 bytes)/24 bytes per LED = 2728 LEDs.
 */

#define LED_COUNT           COLUMBS*ROWS
#define LED_BUFFER_SIZE     24*LED_COUNT+42         // Buffer size needs to be the number of LEDs times 24 bits plus 42 trailing bit to signify the end of the data being transmitted.

extern uint8_t rgb[LED_COUNT][3];                   //Array that will store color data
extern uint8_t led_Colors[LED_COUNT];               //Array of integres that will function as indexes for the rgb array
extern uint16_t ledBuff[LED_BUFFER_SIZE];           //Array of data to be sent to leds.
extern uint32_t usart_Baud_Rate;
extern char usartBTBuffer[25];
extern bool config_BT;

extern char usartBuff[LED_BUFFER_SIZE+100];         // Debugging buffer

