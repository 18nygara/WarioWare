/*
 *  task_minigame2.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef TASK_MINIGAME2_H_
#define TASK_MINIGAME2_H_

#define I2C_TEMP_ADDR   0x40

#define I2C_TEMP_VOBJ           0x00
#define I2C_TEMP_AMBIENT_TEMP   0x01
#define I2C_TEMP_CONFIG         0x02
#define I2C_TEMP_DEV_ID         0xFF

/* CONFIGURATION REGISTER SETTINGS */
#define TMP006_RST              0x8000
#define TMP006_POWER_DOWN       0x0000
#define TMP006_POWER_UP         0x7000
#define TMP006_CR_4             0x0000
#define TMP006_CR_2             0x0200
#define TMP006_CR_1             0x0400
#define TMP006_CR_0_5           0x0600
#define TMP006_CR_0_25          0x0800
#define TMP006_EN               0x0100
#define TMP006_DRDY             0x0080

#include <main.h>

extern TaskHandle_t Task_Minigame_2_Handle;
extern float temp;
extern float prev_temp;

void Task_Minigame_2_Init(void);

/******************************************************************************
 * Returns the current temperature in degrees C
 ******************************************************************************/
float read_temp(void);

/*******************************************************************************
 * Draw the appropriate image to the screen and turn on the appropriate LED based
 * on the temp value
 ******************************************************************************/
void display_temp(float temp);

/******************************************************************************
 * Task that runs the temperature sensor minigame
 ******************************************************************************/
void Task_Minigame_2(void *pvParameters);

#endif /* TASK_MINIGAME2_H_ */
