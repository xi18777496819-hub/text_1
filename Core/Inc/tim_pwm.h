/* tim_pwm.h */ /* 文件：TIM PWM 驱动头文件，声明对外接口 */
#ifndef __TIM_PWM_H /* 头文件保护宏开始，防止重复包含 */
#define __TIM_PWM_H /* 定义保护宏 */

#include "stm32f1xx_hal.h" /* 包含 STM32F1 HAL 库的公共头文件，提供 HAL 类型 */

/* 对外暴露的初始化与设置接口（API） */
void TIM_PWM_Init(void); /* 函数声明：初始化 TIM PWM 外设并配置通道 */
void TIM_PWM_SetDuty(float ch1, float ch2, float ch3, float ch4); /* 函数声明：设置四路占空比，参数范围 0.0~1.0 */

#endif /* __TIM_PWM_H */ /* 头文件保护宏结束 */
