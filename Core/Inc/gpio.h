/* gpio.h */ /* 文件：通用 GPIO 初始化头文件（业务扩展用） */
#ifndef __APP_GPIO_H /* 头文件保护宏开始 */
#define __APP_GPIO_H /* 定义保护宏 */

#include "stm32f1xx_hal.h" /* 包含 HAL 公共头，提供 GPIO 类型和宏 */

/* 初始化与配置通用 GPIO 的接口声明（对外可见） */
void APP_GPIO_Init(void); /* 声明：应用层通用 GPIO 初始化函数，CubeMX 的 MX_GPIO_Init 之外的扩展 */

#endif /* __APP_GPIO_H */ /* 头文件保护宏结束 */
