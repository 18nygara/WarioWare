/*
 *  main.h
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#ifndef MAIN_H_
#define MAIN_H_

/* GENERAL INCLUDE STATEMENTS */
#include "msp.h"
#include "msp432p401r.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/* ECE353 Includes */
#include <ece353.h>
#include <music.h>
#include <task_joystick.h>

/* TASK HEADER INCLUDES */
#include <task_driver.h>
#include <task_minigame1.h>
#include <task_minigame2.h>
#include <task_minigame3.h>
#include <task_minigame4.h>
#include <task_minigame5.h>
#include <task_minigame6.h>
#include <task_timer.h>
#include <task_console.h>
#include <i2c.h>
#include <task_joystick.h>

/* bitmap includes */
#include <title_bitmap.h>
#include <heart_bitmap.h>
#include <bomb_bitmap.h>
#include <broken_heart.h>
#include <explosion.h>
#include <lcd.h>
#include <warm_guy.h>
#include <cold_guy.h>
#include <sleeping.h>
#include <awake.h>
#include <space_ship.h>
#include <cow.h>
#include <arrow.h>
#include <bottle.h>
#include <splash.h>

#endif /* MAIN_H_ */
