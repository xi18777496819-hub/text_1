/* light_app.c */
#include "light_app.h" /* 业务层头文件 */
#include "tim_pwm.h" /* 底层 PWM 输出接口 */

/* 静态内部状态，外部不可见 */
static uint16_t target_cct = 4000; /* 目标色温，默认 4000K */
static float target_brightness = 0.8f; /* 目标亮度，默认 80% */

/* 初始化业务层，初始化混光与底层驱动 */
void LightApp_Init(void)
{
    LightMix_Init(); /* 调用混光模块初始化 */
    TIM_PWM_Init();  /* 启动 PWM 输出 */
}

/* 设置色温接口 */
void LightApp_SetColorTemperature(uint16_t cct)
{
    target_cct = cct; /* 保存目标色温 */
}

/* 设置亮度接口 */
void LightApp_SetBrightness(float brightness)
{
    if (brightness < 0.0f) brightness = 0.0f; /* 限幅 */
    if (brightness > 1.0f) brightness = 1.0f; /* 限幅 */
    target_brightness = brightness; /* 保存亮度 */
}

/* 业务循环调用：计算占空比并下发到底层 */
void LightApp_Update(void)
{
    LightPWM_t pwm = LightMix_Calc(target_cct, target_brightness); /* 计算占空比 */

    /* 将算法输出映射到底层 PWM 接口，接口期望 0~1.0 */
    TIM_PWM_SetDuty(pwm.cold, pwm.warm, pwm.red, pwm.green);
}
