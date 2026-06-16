/* light_app.h */ /* 文件：业务逻辑层头文件，封装混光业务接口 */
#ifndef __LIGHT_APP_H /* 头文件保护宏开始 */
#define __LIGHT_APP_H /* 定义保护宏 */

#include "stm32f1xx_hal.h" /* HAL 基本类型 */
#include "light_mix.h" /* 引入混光算法的类型与接口 */

/* 业务层对外接口声明：初始化、设置与更新 */
void LightApp_Init(void); /* 初始化业务模块（初始化混光、PWM 驱动等） */
void LightApp_SetColorTemperature(uint16_t cct); /* 设置目标色温，单位 Kelvin */
void LightApp_SetBrightness(float brightness); /* 设置亮度（0.0 ~ 1.0） */
void LightApp_Update(void); /* 业务循环调用接口：计算并下发 PWM */

#endif /* __LIGHT_APP_H */ /* 头文件保护宏结束 */
