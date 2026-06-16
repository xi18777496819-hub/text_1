/* light_mix.h */ /* 文件：混光算法头文件，声明色温->PWM 的接口 */
#ifndef __LIGHT_MIX_H /* 头文件保护宏开始 */
#define __LIGHT_MIX_H /* 定义保护宏 */

#include "stm32f1xx_hal.h" /* 包含 HAL 类型定义（用于 uint16_t 等基础类型） */

/* PWM 输出结构体：表示四路占空比，取值范围 0.0 ~ 1.0 */
typedef struct {
    float cold;  /* 冷白通道占空比（0~1） */
    float warm;  /* 暖白通道占空比（0~1） */
    float red;   /* 红光通道占空比（0~1） */
    float green; /* 绿光通道占空比（0~1） */
} LightPWM_t; /* 定义混光输出结构体类型 */

/* 初始化混光模块（例如加载校准表），目前为保留入口 */
void LightMix_Init(void); /* 对外初始化接口 */

/* 核心计算接口：根据输入色温（K）和亮度（0~1）返回四路占空比 */
LightPWM_t LightMix_Calc(uint16_t cct, float brightness); /* 计算并返回占空比 */

#endif /* __LIGHT_MIX_H */ /* 头文件保护宏结束 */
