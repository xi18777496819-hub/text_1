/* gpio.h */
#ifndef __APP_GPIO_H
#define __APP_GPIO_H

#include "stm32f1xx_hal.h"

/* 初始化与配置通用 GPIO 的接口声明 */
void APP_GPIO_Init(void); /* 由 CubeMX 以外的代码调用的 GPIO 初始化 */

#endif /* __APP_GPIO_H */
