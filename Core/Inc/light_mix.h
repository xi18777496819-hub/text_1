#ifndef __LIGHT_MIX_H
#define __LIGHT_MIX_H

#include <stdint.h>

typedef struct
{
    float ch1;   // 冷白
    float ch2;   // 暖白
    float ch3;   // 红光
    float ch4;   // 蓝光
} PWM_Output_t;

typedef enum
{
    MODE_DAY = 0,
    MODE_NIGHT,
    MODE_RELAX
} Light_Mode_t;

void Light_SetMode(Light_Mode_t mode);
void Light_SetColorTemp(uint16_t cct);
PWM_Output_t Light_GetPWM(void);
void Light_SetBrightness(float brightness); // 0.0 ~ 1.0

#endif // __LIGHT_MIX_H
