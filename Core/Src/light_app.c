/* light_app.c */ /* 文件：业务层实现，负责将算法与驱动连接 */
#include "light_app.h" /* 包含业务层头文件 */
#include "tim_pwm.h"   /* 包含底层 TIM PWM 驱动接口 */

/* 静态内部状态（模块私有），用于保存目标色温与亮度 */
static uint16_t target_cct = 4000;      /* 目标色温，默认 4000K（中性白） */
static float target_brightness = 0.8f;  /* 目标亮度，默认 80% */

/* 初始化业务层：初始化混光算法模块和 PWM 驱动 */
void LightApp_Init(void)
{
    LightMix_Init(); /* 初始化混光算法（表、校准等） */
    TIM_PWM_Init();  /* 初始化并启动 TIM PWM 输出 */
}

/* 设置目标色温接口：更新模块内部状态 */
void LightApp_SetColorTemperature(uint16_t cct)
{
    target_cct = cct; /* 将外部传入的色温保存为目标色温 */
}

/* 设置亮度接口：设置总亮度并进行限幅处理 */
void LightApp_SetBrightness(float brightness)
{
    if (brightness < 0.0f)
        brightness = 0.0f; /* 限制最小值 */
    if (brightness > 1.0f)
        brightness = 1.0f; /* 限制最大值 */
    target_brightness = brightness; /* 保存有效的亮度值 */
}

/* 业务周期性运行函数：计算混光占空比并下发到底层 PWM 驱动 */
void LightApp_Update(void)
{
    LightPWM_t pwm = LightMix_Calc(target_cct, target_brightness); /* 计算当前占空比 */

    /* 将算法输出转为底层驱动可接受的占空比值（0~1），并下发 */
    TIM_PWM_SetDuty(pwm.cold, pwm.warm, pwm.red, pwm.green);
}
