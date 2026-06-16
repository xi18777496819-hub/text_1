/* light_app.h */
#ifndef __LIGHT_APP_H
#define __LIGHT_APP_H

#include "stm32f1xx_hal.h"
#include "light_mix.h"

/* 业务层接口：设置色温与亮度 */
void LightApp_Init(void); /* 初始化业务模块 */
void LightApp_SetColorTemperature(uint16_t cct); /* 设置目标色温（K） */
void LightApp_SetBrightness(float brightness); /* 设置总亮度 0~1.0 */
void LightApp_Update(void); /* 业务循环调用，执行混光并下发 PWM */

#endif /* __LIGHT_APP_H */
