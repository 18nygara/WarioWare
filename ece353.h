/*
 *  ece353.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef __ECE353_H__
#define __ECE353_H__

#include <stdbool.h>
#include <stdint.h>
#include "msp.h"

/*
 * Initialize hardware resources used to control LED1
 */
void ece353_init_led1(void);

/*
 * Initialize hardware resources used to control Button1
 */
void ece353_init_button1(void);

/*
 * Turn LED1 on/off
 * 
 * Parameters
 *  on: if true, turn LED on
 *      if false, turn LED off
 */
void ece353_led1(bool on);

/*
 * Returns if Button1 us currently pressed.
 * 
 * Parameters
 *  true: Button1 is pressed
 *  false: Button1 is not pressed
 */
bool ece353_button1(void);

/*
 * Initialize hardware resources used to control RGBLED
 */
void ece353_init_rgbled(void);

/*
 * Initialize hardware resources used to control Button2
 */
void ece353_init_button2(void);

/*
 * Returns if Button2 us currently pressed.
 * 
 * Parameters
 *  true: Button2 is pressed
 *  false: Button2 is not pressed
 */
bool ece353_button2(void);

/*
 * Turn RGBLED RED on/off
 * 
 * Parameters
 *  on: if true, turn RGBLED RED on
 *      if false, turn RGBLED RED off
 */
void ece353_rgbled_red(bool on);

/*
 * Turn RGBLED BLUE on/off
 * 
 * Parameters
 *  on: if true, turn RGBLED BLUE on
 *      if false, turn RGBLED BLUE off
 */
void ece353_rgbled_blue(bool on);

/*
 * Turn RGBLED GREEN on/off
 * 
 * Parameters
 *  on: if true, turn RGBLED GREEN on
 *      if false, turn RGBLED GREEN off
 */
void ece353_rgbled_green(bool on);

/*
 * Turn off all RGBLEDS
 */
void ece353_rgbled_off();

void ece353_T32_1_wait(uint32_t ticks);

void ece353_T32_1_wait_100mS(void);

void ece353_button1_wait_for_press(void);

void ece353_MKII_S1_Init(void);

void ece353_MKII_S2_Init(void);

bool ece353_MKII_S1(void);

bool ece353_MKII_S2(void);

void ece353_MKII_Buzzer_Init(uint16_t ticks_period);

void ece353_MKII_Buzzer_On(void);

void ece353_MKII_Buzzer_Off(void);

bool ece353_MKII_Buzzer_Run_Status(void);

/*****************************************************
 * Initialize the 3 GPIO pins that control the RGB
 * LED on the MKII.
 *
 * Parameters
 *      None
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_IO_Init(bool en_primary_function);

/*****************************************************
 * Sets the PWM levels for the MKII RGBLED
 *
 * Parameters
 *      ticks_period    :   Period of PWM Pulse
 *      ticks_red_on    :   Number of Ticks RED is on
 *      ticks_green_on  :   Number of Ticks GREEN is on
 *      ticks_blue_on   :   Number of Ticks BLUE is on
 * Returns
 *      None
 *****************************************************/
void ece353_MKII_RGB_PWM(
        uint16_t ticks_period,
        uint16_t ticks_red_on,
        uint16_t ticks_green_on,
        uint16_t ticks_blue_on
);

void ece353_T32_1_Interrupt_Ms(uint16_t ms);

void ece353_ADC14_PS2_X(void);

void ece353_ADC14_PS2_XY(void);

void ece353_MKII_RGB_LED(bool red, bool green, bool blue);

void ece353_RBG_LED_Init(void);

void ece353_RGB_LED(bool red, bool green, bool blue);

void ece353_RBG_LED_Init(void);

void ece353_ADC14_PS2_XY_COMP(void);

void ece353_RBG_LED_Init();

void ece353_change_PWM_period(uint16_t ticks_period);

#endif
