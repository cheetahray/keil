/* ---------------------------------------------------------------------------- */
/*                                                                              */
/*  NAME:       YOUSSIF MAHJOUB                                                 */
/*  DATE:       5/11/16                                                         */
/*                                                                              */
/*  PROJECT:    WS2812 LED MATRIX DRIVEN BY STM32F103C8T6                       */
/*                                                                              */
/*  DESCRIPTION:                                                                */
/*      THIS SCRIPT DRIVES A WS2812 LED MATRIX. IT USES A TIMER TO              */
/*      GENERATE A PWM SIGNAL. THIS PWM SIGNAL HAS THE REQUIRED DATA            */
/*      AND CLOCK EMBEDDED INTO IT. IN ORDER TO KEEP THE TIMINGS CORRECT        */
/*      THE CPU FEEDS THE DMA CONTROLLER WITH THE PWM WIDTH DATA AND THE        */
/*      DMA CONTROLLER MANIPULATES THE TIMER.                                   */
/*                                                                              */
/*  NOTES:                                                                      */
/*                                                                              */
/*  FILE:       MAIN.C                                                          */
/*                                                                              */
/* ---------------------------------------------------------------------------- */


// System includes
#include <stm32f10x.h>
#include <stdio.h>                                                  // For sprintf()
#include <stdlib.h>                                                 // For rand()
#include <math.h>                                                   // For floor()
#include <string.h>                                                 // For memset()
#include <stdbool.h>                                                // For Boolean data type

// User includes
#include "vars.h"                                                   // Global Variables
#include "config.h"                                                 // Functions used to configure peripherals 

// Function Prototypes
void HC06BT_Config(void);
u16 readADC1(u8 channel);
void send_data(uint8_t (*led_Colors), uint16_t len);
void test_LED(float r, float g, float b);
void random_Noise(float r, float g, float b);
void converge_Center(float r, float g, float b);
void rainbow_Loop(void);
void test_Matrix(void);
void rotary_Matrix(uint8_t *nowho, uint8_t *restone, uint8_t *restwo, uint8_t *direction, int16_t *idx, float *speed);
// Sloppy delay function (not accurate)
void Delay(__IO uint32_t nCount) { while(nCount--) { } }

int main() {
    
    uint8_t nowho=1, direction=0, restone=255, restwo=255;
    int16_t idx=(COLUMBS>>1);
    float speed = 0.0;
    //config_BT = true;
    config_BT = false;
    
    if(config_BT) {
        usart_Baud_Rate = 115200;
        Peripheral_Config();
        HC06BT_Config();
    } else {
        
        uint16_t animation_Select;
        uint16_t i;//, j = 0, adc_Value;
        
        // Configure peripherals
        usart_Baud_Rate = 115200;
        Peripheral_Config();

        // Reset LEDs
        send_data(led_Colors, LED_COUNT);

        animation_Select = 5;

        while (1){  
            
            //while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {}
            //temp = USART_ReceiveData(USART1);
            
                
            switch(animation_Select) {
                case(0) : { test_LED(0, 1, 0); break;}
                case(1) : { random_Noise(1, 1, 1); break;}
                case(2) : { converge_Center(0.1, 0, 0); break; }
                case(3) : { rainbow_Loop(); break; }
                case(4) : { test_Matrix(); break; }
                case(5) : { rotary_Matrix(&nowho, &restone, &restwo, &direction, &idx, &speed); break; }
            }
            /*
            memset(&usartBTBuffer, '\0', sizeof(usartBTBuffer));
            sprintf(usartBTBuffer, "Test %d\r\n", j++);
                
            for(i = 0; i < sizeof(usartBTBuffer); i++) {
                if(usartBTBuffer[i] != '\0') {
                    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
                    USART_SendData(USART1, usartBTBuffer[i]);
                } else {
                    break; // Exit loop if NULL found
                }
            }*/
            //for(i = 0; i < 1*10; i++) { Delay(50000L); }
        } //While(1) l[oop
    } //config_BT If statement                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
}

//This function send AT commands over usart 1 PA9 to configure the HC-06 Bluetooth module
void HC06BT_Config(void) {
    
    
    int i;
    char atCMD[19];
    
    //Clear strange data
    memset(&atCMD, '\0', sizeof(atCMD));
    for(i = 0; i < sizeof(atCMD); i++) {
        if(atCMD[i] != '\0') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
            USART_SendData(USART1, atCMD[i]);
        } else {
            break;
        }
    }
    for(i = 0; i < 25*10; i++) { Delay(50000L); }
    
    //Set name
    memset(&atCMD, '\0', sizeof(atCMD));
    sprintf(atCMD, "AT+NAMEDAFTPUNK_HELM");
    for(i = 0; i < sizeof(atCMD); i++) {
        if(atCMD[i] != '\0') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
            USART_SendData(USART1, atCMD[i]);
        } else {
            break;
        }
    }
    for(i = 0; i < 50*10; i++) { Delay(50000L); }
    
    //---Set pin number--------------------------------
    memset(&atCMD, '\0', sizeof(atCMD));
    sprintf(atCMD, "AT+PIN7060");
    for(i = 0; i < sizeof(atCMD); i++) {
        if(atCMD[i] != '\0') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
            USART_SendData(USART1, atCMD[i]);
        } else {
            break;
        }
    }
    for(i = 0; i < 50*10; i++) { Delay(50000L); }
    
    //---Set baud------------------------------
    memset(&atCMD, '\0', sizeof(atCMD));
    sprintf(atCMD, "AT+BAUD8");
    for(i = 0; i < sizeof(atCMD); i++) {
        if(atCMD[i] != '\0') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
            USART_SendData(USART1, atCMD[i]);
        } else {
            break;
        }
    }
}


//u16 readADC1(u8 channel) {
//  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_1Cycles5);
//  // Start the conversion
//  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//  // Wait until conversion completion
//  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
//  // Get the conversion value
//  return ADC_GetConversionValue(ADC1);
//}

void send_data(uint8_t *led_Colors, uint16_t len) {
    
    
    uint8_t i, j;//, k = 0;
    uint16_t buffersize = (24*len)+42, memaddr = 0;

    
    uint8_t temp, led = 0;
    
    while(len) {        
        for (i = 0; i < 3; i++) {                                   // Set RGB LED color R -> i=0, G -> i=1, B -> i=2
            temp = rgb[led_Colors[led]][i];
            for (j = 0; j < 8; j++) {                               // Set 8 bits of color
                if ((temp) & 0x80) {                                // Data sent MSB first, j = 0 is MSB j = 7 is LSB   
                    ledBuff[memaddr++] = PWM_HIGH_WIDTH;            // Compare value for logical 1
                    // ### DEBUGGING - usartBuff[k++] = '1';
                } else {
                    ledBuff[memaddr++] = PWM_LOW_WIDTH;             // Compare value for logical 0
                    // ### DEBUGGING - usartBuff[k++] = '0';
                }   
                    temp = temp << 1;
            }
            // ### DEBUGGING - if(i < 2)
            // ### DEBUGGING -  usartBuff[k++] = '-';
        }
    
        // ### DEBUGGING - usartBuff[k++] = ' ';
        // ### DEBUGGING - usartBuff[k++] = '|';
        // ### DEBUGGING - usartBuff[k++] = ' ';
        
        led++;
        len--;
    }
    
    // ### DEBUGGING - usartBuff[LED_BUFFER_SIZE+50] = '\n';
    // ### DEBUGGING - usartBuff[LED_BUFFER_SIZE+51] = '\r';
    
    // Add needed delay at end of byte cycle, pulsewidth = 0
    while(memaddr < buffersize) {
        ledBuff[memaddr++] = 0;
    }
        
    DMA_SetCurrDataCounter(DMA1_Channel1, LED_BUFFER_SIZE);             // Load number of bytes to be transferred
    DMA_Cmd(DMA1_Channel1, ENABLE);                                     // Enable DMA channel 1
    TIM_Cmd(TIM2, ENABLE);                                              // Enable Timer 2
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));                           // Wait until transfer complete
    TIM_Cmd(TIM2, DISABLE);                                             // Disable Timer 2
    DMA_Cmd(DMA1_Channel1, DISABLE);                                    // Disable DMA channel 1
    DMA_ClearFlag(DMA1_FLAG_TC1);                                       // Clear DMA1 Channel 1 transfer complete flag
}

void rotaryUart(char * arg0, uint16_t arg1) {
    int i;
    char atCMD[8];
    memset(&atCMD, '\0', sizeof(atCMD));
    sprintf(atCMD, "%s %d\t", arg0, arg1); //, direction_read);
    for(i = 0; i < sizeof(atCMD); i++) {
        if(atCMD[i] != '\0') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
            USART_SendData(USART1, atCMD[i]);
        } else {
            break;
        }
    }
}

void rotaryfUart(char * arg0, float arg1) {
    int i;
    char atCMD[16];
    memset(&atCMD, '\0', sizeof(atCMD));
    sprintf(atCMD, "%s %f\t", arg0, arg1); //, direction_read);
    for(i = 0; i < sizeof(atCMD); i++) {
        if(atCMD[i] != '\0') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
            USART_SendData(USART1, atCMD[i]);
        } else {
            break;
        }
    }
}

uint16_t calAngle() {
    uint16_t retAngle;
    retAngle = _2_0_read + (_2_1_read << 1) + (_2_2_read << 2) + (_2_3_read << 3) + (_2_4_read << 4)
                         + (_2_5_read << 5) + (_2_6_read << 6) + (_2_7_read << 7);
    retAngle = ( ( retAngle * 45 ) >> 5 ) ;
    return retAngle;
}
    
void test_LED(float r, float g, float b) {
    
    uint8_t i, j, k, brightness = 255;
    
    for(i = 0; i < 4; i++) {
        rgb[0][0] = 0;
        rgb[0][1] = 0;
        rgb[0][2] = 0;
        if(i == 3) {
            rgb[0][0] = brightness;
            rgb[0][1] = brightness;
            rgb[0][2] = brightness;
        } else {
                rgb[0][i] = brightness;
        }
        for(j = 0; j < LED_COUNT; j++)
            led_Colors[j] = 0;
        
        // Send data to LEDs
        send_data(led_Colors, LED_COUNT);   
    
        for(k = 0; k < 5*10; k++) {
            Delay(50000L);
        }
    }
    
    
}


/********random_Noise********random_Noise********random_Noise********random_Noise********random_Noise********
* Purpose:          Fluctuate LED brightness to simulate noise.
* Parameters:       Three floats that range from [0,1]. These values represent 
*                               how bright a specific color should be. This allows us to 
*                               chose whatever color of noise we want.
*   Note:                   This function can easily turn all the LED a specific color
*                               with a constant brightness, such as full brightness white
*                               which requires the mose current, by simply setting the 
*                               "noise" value which ranges from [0,255].
*/
void random_Noise(float r, float g, float b) {
    
    uint8_t i, noise;
    
    for(i = 0; i < LED_COUNT; i++) {
        noise = rand() % 255;
        rgb[i][0] = (uint8_t)floor(r*noise);
        rgb[i][1] = (uint8_t)floor(g*noise);
        rgb[i][2] = (uint8_t)floor(b*noise);
        led_Colors[i] = 0;//rand() % LED_COUNT+1;
    }
    
    // Send data to LEDs
    send_data(led_Colors, LED_COUNT);   
    
    for(i = 0; i < 0.5*10; i++) {
        Delay(50000L);
    }
}
/********END random_Noise********END random_Noise********END random_Noise********END random_Noise********/

void converge_Center(float r, float g, float b) {
    uint8_t i, j, k;//, temp = 0;
    
    rgb[0][0] = 0;
    rgb[0][1] = 0;
    rgb[0][2] = 0;
    rgb[1][0] = (uint8_t)floor(r*255);
    rgb[1][1] = (uint8_t)floor(g*255);
    rgb[1][2] = (uint8_t)floor(b*255);
    
    
    for(i = 0; i < COLUMBS/2+1; i++) {
        if(i == COLUMBS/2){
            for(k = 0; k < LED_COUNT; k++) {
                led_Colors[k] = 0;
            }
        }   else {
            for(j = 0; j < ROWS; j++) {
                led_Colors[j*COLUMBS+i] = 1;    
                led_Colors[(j*COLUMBS)+7-i] = 1;            
            }       
        }
        // Send data to LEDs
        send_data(led_Colors, LED_COUNT);   
        for(j = 0; j < 2*10; j++) {
            Delay(50000L);
        }
    }
}

void rainbow_Loop(){
    
    uint8_t j, intStageNum = 0, r = 255, g = 0, b = 0;
    uint16_t i;
    
    for(i = 0; i < 255*5; i++) {
        switch (intStageNum) {
            case 0 : { g++; if(g == 255) { intStageNum++;} break; }     // To change from Red to Yellow, just Green needs to go from 0 to 255 so we do this
            case 1 : { r--; if(r == 0)   { intStageNum++;} break; }     // we are now at (255, 255, 0) yellow, to go to green we need to reduce red to 0
            case 2 : { b++; if(b == 255) { intStageNum++;} break; }
            case 3 : { g--; if(g == 0)   { intStageNum++;} break; }     // now at (0, 255, 255)... for blue reduce green to 0
            case 4 : { r++; if(r == 255) { intStageNum++; break;} }     // now at (0, 0, 255)... for magenta increase red to 255
            case 5 : { b--; if(b == 0)   { intStageNum++;} break; }     // now at (255, 0, 255)... and back to red reduce B to 0
            default : intStageNum = 0;
        }
        rgb[0][0] = (uint8_t)floor(r);
        rgb[0][1] = (uint8_t)floor(g);
        rgb[0][2] = (uint8_t)floor(b);  
        led_Colors[0] = 0;
        // Send data to LEDs
        send_data(led_Colors, LED_COUNT);   
        for(j = 0; j < 0.001*10; j++) {
            Delay(50000L);
        }
     }
}

void test_Matrix() {
    
    uint8_t i;//, j;
    
    rgb[0][0] = 0;
    rgb[0][1] = 0;
    rgb[0][2] = 0;
    
    rgb[1][0] = 128;
    rgb[1][1] = 128;
    rgb[1][2] = 128;
    
    for(i = 0; i < LED_COUNT; i++) {
        led_Colors[LED_COUNT-1] = 0;
        led_Colors[i] = 1;
        if(i > 0)
            led_Colors[i-1] = 0;
        
        
        send_data(led_Colors, LED_COUNT);   
        /*
        for(j = 0; j < 0.6*10; j++) {
            Delay(50000L);
        }
        */
        calAngle();
    }        
    
}

void calspeed(float *x, float *v, float a)
{
    //float t = 1.7;
    (*x) = (*v) /**t*/ + ( a /** (t*t)*/ / 2.0 );
    (*v) = (*v) + a /** t*/;
}

void rotary_Matrix(uint8_t *nowho, uint8_t *restone, uint8_t *restwo, uint8_t *direction, int16_t *idx, float *speed) {
    
    uint8_t i, howmanyled=5;//, j;
    float x=0.0, divider = 20.0;
    uint16_t retAngle=calAngle();
    //uint16_t idx= (COLUMBS-howmanyled)*retAngle/360;
    if (retAngle >=175 && retAngle <= 185)
    {
        (*speed) = 0.0;
    }
    else if (retAngle >=186 && retAngle <= 191)
    {
        calspeed(&x, speed, 1.0 / divider );
    }
    else if(retAngle >=192 && retAngle <= 197)
    {
        calspeed(&x, speed, 2.0 / divider );
    }
    else if(retAngle >=198 && retAngle <= 203)
    {
        calspeed(&x, speed, 3.0 / divider );
    }
    else if(retAngle >=204 && retAngle <= 209)
    {
        calspeed(&x, speed, 4.0 / divider );
    }
    else if(retAngle >=210 && retAngle <= 216)
    {
        calspeed(&x, speed, 5.0 / divider );
    }
    else if(retAngle >=217 && retAngle <= 224)
    {
        calspeed(&x, speed, 6.0 / divider );
    }
    else if(retAngle >=225 && retAngle <= 233)
    {
        calspeed(&x, speed, 7.0 / divider );
    }
    else if(retAngle >=234 && retAngle <= 255)
    {
        calspeed(&x, speed, 8.0 / divider );
    }
    else if(retAngle >=256 && retAngle <= 269)
    {
        calspeed(&x, speed, 9.0 / divider );
    }
    else if (retAngle <=174 && retAngle >= 169)
    {
        calspeed(&x, speed, -1.0 / divider );
    }
    else if(retAngle <=168 && retAngle >= 163)
    {
        calspeed(&x, speed, -2.0 / divider );
    }
    else if(retAngle <=162 && retAngle >= 157)
    {
        calspeed(&x, speed, -3.0 / divider );
    }
    else if(retAngle <=156 && retAngle >= 151)
    {
        calspeed(&x, speed, -4.0 / divider );
    }
    else if(retAngle <=150 && retAngle >= 144)
    {
        calspeed(&x, speed, -5.0 / divider );
    }
    else if(retAngle <=143 && retAngle >= 136)
    {
        calspeed(&x, speed, -6.0 / divider );
    }
    else if(retAngle <=135 && retAngle >= 127)
    {
        calspeed(&x, speed, -7.0 / divider );
    }
    else if(retAngle <=126 && retAngle >= 105)
    {
        calspeed(&x, speed, -8.0 / divider );
    }
    else if(retAngle <=104 && retAngle >= 91)
    {
        calspeed(&x, speed, -9.0 / divider );
    }
    else if(retAngle >=270 && retAngle <= 359)
    {
        calspeed(&x, speed, 10.0 / divider );
    }
    else
    {
        calspeed(&x, speed, -10.0 / divider );
    }
    
    rgb[0][0] = 0;
    rgb[0][1] = 0;
    rgb[0][2] = 0;

    if(1 == *nowho)
    {
        rgb[1][0] = 255;
        rgb[1][1] = (uint8_t)(*restone);
        rgb[1][2] = (uint8_t)(*restwo);
    }
    else if(2 == *nowho)
    {
        rgb[1][0] = 255;
        rgb[1][1] = (uint8_t)(*restone);
        rgb[1][2] = (uint8_t)(*restwo);
    }
    else if(3 == *nowho)
    {
        rgb[1][0] = (uint8_t)(*restone);
        rgb[1][1] = 255;
        rgb[1][2] = (uint8_t)(*restwo);
    }

    if(0 == *direction)
    {
        (*restwo)--;
        if(0 == *restwo)
            *direction = 1;
    }
    else
    {
        (*restwo)++;
        if(255 == *restwo)
        {
            *direction = 0;
            (*restone)--;
            if(0 == *restone)
            {
                *restone = 255;
                (*nowho)++;
                if(4 == *nowho)
                    *nowho = 1;
            }
        }
    }    
    rotaryUart("1.", retAngle);
    rotaryUart("2.", *idx);
    /*
    rotaryUart("3.", *nowho);
    rotaryUart("4.", *restone);
    rotaryUart("5.", *restwo);
    rotaryUart("6.", *direction);
    */
    rotaryfUart("7.", x);

    (*idx)+=(int16_t)x;
    if(*idx < 0)
    {
        *idx = 0;
        if(x <= -3.0)
            (*speed) *= -0.815;
        else if(x <= -2.0)
            (*speed) *= -0.7;
        else if(x <= -1.0)
            (*speed) *= -0.5;
    }
    else if (*idx >= COLUMBS-1-howmanyled)
    {
        *idx = COLUMBS-2-howmanyled;
        if(x >= 3.0)
            (*speed) *= -0.815;
        else if(x >= 2.0)
            (*speed) *= -0.7;
        else if(x >= 1.0)
            (*speed) *= -0.5;
    }    
    
    for(i = 0; i < LED_COUNT; i++) 
    {
        if (i < (*idx) || ( i >= (*idx)+howmanyled && i < ( LED_COUNT-(*idx)-(howmanyled<<1) ) ) || i >= (LED_COUNT-(*idx)-howmanyled) )
            led_Colors[i] = 0;
        else
            led_Colors[i] = 1;
    }
    send_data(led_Colors, LED_COUNT);   
    
}
