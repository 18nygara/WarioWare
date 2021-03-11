/*
 * Authors: Collin Krause and Adam Nygard
 *
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "main.h"

/*
 *  ======== main ========
 */
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // call init functions
    music_init();
    Task_Driver_Init();
    Task_Console_Init();
    Task_Joystick_Init();
    i2c_init();
    Crystalfontz128x128_Init();
    Task_Minigame_1_Init();
    Task_Minigame_2_Init();
    Task_Minigame_3_Init();
    Task_Minigame_4_Init();
    Task_Minigame_5_Init();
    Task_Minigame_6_Init();

    // enable interrupts
    __enable_irq();

    //create tasks here
    xTaskCreate(task_music_play_song, "Task_Play_Song",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Play_Song_Handle);

    xTaskCreate(Task_Driver, "Task_Driver",
    configMINIMAL_STACK_SIZE,
                NULL, 3, &Task_Driver_Handle);

    xTaskCreate(Task_Minigame_1, "Light Sensor Task",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Minigame_1_Handle);

    xTaskCreate(Task_Minigame_2, "TMP sensor Task",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Minigame_2_Handle);

    xTaskCreate(Task_Minigame_3, "Joystick Alien Task",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Minigame_3_Handle);

    xTaskCreate(Task_Minigame_4, "Simon Says Task",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Minigame_4_Handle);

    xTaskCreate(Task_Minigame_5, "Accelerometer Task",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Minigame_5_Handle);

    xTaskCreate(Task_Minigame_6, "Color picker Task",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Minigame_6_Handle);

    xTaskCreate(Task_Timer, "Task_Timer",
    configMINIMAL_STACK_SIZE,
                NULL, 2, &Task_Timer_Handle);

    xTaskCreate(Task_Joystick_Timer, "Task_Joystick_Timer",
        configMINIMAL_STACK_SIZE,
                    NULL, 2, &Task_Joystick_Timer_Handle);

    xTaskCreate(Task_Joystick_Bottom_Half, "Task_Joystick_Bottom_Half",
        configMINIMAL_STACK_SIZE,
                    NULL, 3, &Task_Joystick_Handle);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    while (1)
    {
    };
    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while (1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while (1)
    {
    }
}
