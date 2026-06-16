/* light_mix.h */
#ifndef __LIGHT_MIX_H
#define __LIGHT_MIX_H

#include "stm32f1xx_hal.h"

/* PWM 输出结构体，四路占空比 0~1.0 */
typedef struct
{
    float cold; /* 冷白 占空比 */
    float warm; /* 暖白 占空比 */
    float red;  /* 红光 占空比 */
    float green;/* 绿光 占空比 */
} LightPWM_t;

/* 初始化混光模块（如需校准） */
void LightMix_Init(void);

/* 根据色温和亮度计算四路占空比，色温单位 K，范围 3000~6500，亮度 0~1.0 */
LightPWM_t LightMix_Calc(uint16_t cct, float brightness);

#endif /* __LIGHT_MIX_H */
