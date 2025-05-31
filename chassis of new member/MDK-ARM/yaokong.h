#ifndef _YAOKONG_H_
#define _YAOKONG_H_
#include "main.h"
typedef struct remote {
    struct
    {
        int16_t ch0_int16;
        int16_t ch0_int16_last;
        int16_t ch1_int16;
        int16_t ch1_int16_last;
        int16_t ch2_int16;
        int16_t ch2_int16_last;
        int16_t ch3_int16;
        int16_t ch3_int16_last;
        int16_t dial_int16;
        uint8_t last_s1;
        uint8_t s1_u8;
        uint8_t last_s2;
        uint8_t s2_u8;
    } control;
} remote_typedef;

extern remote_typedef remote_t;//shengming

void DBUS_Resolve(uint8_t *data);
void DBUS_offLine(void );

#endif