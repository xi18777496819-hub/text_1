/* tim_pwm.h */
#ifndef __TIM_PWM_H
#define __TIM_PWM_H

#include "stm32f1xx_hal.h"

/* 对外暴露的初始化与设置接口 */
void TIM_PWM_Init(void); /* 初始化 TIM PWM 外设 */
void TIM_PWM_SetDuty(float ch1, float ch2, float ch3, float ch4); /* 设置四路占空比，0~1.0 */

#endif /* __TIM_PWM_H */
