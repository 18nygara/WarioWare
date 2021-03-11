/*
 *  arrow.c
 *
 *  Authors: Collin Krause and Adam Nygard
 *  Created on: 12/1/2020
 */

#include <main.h>

int MEASURE_DURATION = 24000000;    // start at 1 second, this will increase over time
QueueHandle_t Queue_Song;
TaskHandle_t Task_Play_Song_Handle;

Note_t MainTheme[] =
{
    {NOTE_C6S,TWELVE},
    {NOTE_G6,FOUR},
    {NOTE_F6S,FOUR},
    {NOTE_E6,FOUR},
    {NOTE_C6S,SEVEN},
    {NOTE_F6S,NINE},
    {NOTE_E6,SIX},
    {NOTE_C6S,TWELVE},
    {NOTE_C6S,FOUR},
    {NOTE_E6,SIXTEEN},
    {NOTE_F6,SIXTEEN},
    {NOTE_F6S,SIXTEEN},
    {NOTE_G6,SIXTEEN},
};

Note_t LosingTheme[] =
{
    {NOTE_D6,SIXTEEN},
    {NOTE_C6S,SIXTEEN},
    {NOTE_C6,SIXTEEN},
    {NOTE_B5,SIXTEEN},
};

Note_t WinningTheme[] =
{
    {NOTE_C6S,TWELVE},
    {NOTE_E6,FOUR},
    {NOTE_F6S,NINE},
    {NOTE_G6,SIXTEEN},
    {NOTE_F6S,NINE},
    {NOTE_E6,SIXTEEN},
};

//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return  = MEASURE_DURATION * MEASURE_RATIO;

    switch(time)
    {
        case TWELVE:
        {
            time_return  = (time_return / 64) * 12;
            break;
        }
        case FOUR:
        {
            time_return  = (time_return / 64) * 4;
            break;
        }
        case SEVEN:
        {
            time_return  = (time_return / 64) * 7;
            break;
        }
        case NINE:
        {
            time_return  = (time_return / 64) * 9;
            break;
        }
        case SIX:
        {
            time_return  = (time_return / 64) * 6;
            break;
        }
        case SIXTEEN:
        {
            time_return  = time_return / 4;
            break;
        }
        default:
        {
            break;
        }
    }
    return time_return - DELAY_AMOUNT;
}


//***************************************************************************
// Plays a single note of the song based on note_index.  After
// the note is played, there is an optional delay in between
// notes.
//
// Examples
// Song[note_index].period       -- Used to determine the period of the
//                                  PWM pulse.
//
// Song[note_index].time         -- 1/4 or 1/2 time note.  Call
//                                  music_get_time_delay() to determine how
//                                  long to play the note
//
// Song[note_index].delay        -- If true, add a period of silence for
//                                  DELAY_AMOUNT
//
//                                  If false, return without delaying.
//*************************************************************************
static void music_play_note(Note_t song[], uint16_t note_index)
{
    Note_t note = song[note_index];
    // change the period of the buzzer
    ece353_change_PWM_period(note.period);
    // play the note, turn buzzer on
    ece353_MKII_Buzzer_On();
    // let the buzzer play for the duration of Song[note_index].time
    ece353_T32_1_wait(music_get_time_delay(note.time));
    // stop the buzzer
    ece353_MKII_Buzzer_Off();
}


//***************************************************************
// Plays the song (loop through, playing each note) 
//***************************************************************
void task_music_play_song(void *pvParameters)
{
    // variable that will decide which winning theme we play
    bool winning;

    while(1) {
        // wait until a queue is received
        xQueueReceive(Queue_Song, &winning, portMAX_DELAY);
        int i;
        // check which song we need to play
        if (winning) {
            for (i = 0; i < 6; i++) { // loop through the song and play every note
                music_play_note(WinningTheme, i);
            }
        } else {
            for (i = 0; i < 4; i++) {
                music_play_note(LosingTheme, i);
            }
        }
        // play the main theme
        for (i = 0; i < 13; i++) {
            music_play_note(MainTheme, i);
        }
        // increase the speed for the next loop, until we reach max speed
        if (MEASURE_DURATION > 8000000) {
            MEASURE_DURATION *= .94;
        }
        // notify to start the next minigame
        xTaskNotifyGive(Task_Driver_Handle);
    }
}

// Initializes TA0 and S1 to be used for this program
void music_init(void) {
    // initialize s1 on the MKII
    ece353_MKII_S1_Init();
    // initialize the buzzer with an arbitrary amount, we will rewrite this period later
    ece353_MKII_Buzzer_Init(100);
    // initialize the queue
    Queue_Song = xQueueCreate(1,sizeof(bool));
}

