/* tim_pwm.c */
#include "tim_pwm.h" /* 包含本模块头文件 */
#include "stm32f1xx_hal.h" /* 包含 HAL 库以获取 TIM 类型与宏 */
#include "main.h" /* 有时需要外部定义的句柄 */

/* 定义 TIM3 句柄，作为本模块私有符号 */
TIM_HandleTypeDef htim3; /* TIM3 句柄，配置为四路 PWM */

/* 本模块提供 TIM_PWM_Init 实现：初始化 TIM3 为 PWM 模式并启动四路输出 */
void TIM_PWM_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* 基本定时器参数：示例设置为 1 kHz PWM（由 PSC 和 ARR 决定） */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 72 - 1; /* 假设系统时钟 72MHz，预分频 72 得到 1MHz 时基 */
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1;  /* 1MHz / 1000 = 1kHz PWM */
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_PWM_Init(&htim3); /* 初始化定时器 PWM 基础功能 */

    /* PWM 输出通道通用配置：占空比初始为 0 */
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0; /* 初始比较值 0，占空比 0% */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);

    /* 启动 PWM 输出通道 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}

/* 设置四路占空比，参数为 0.0f ~ 1.0f 范围 */
void TIM_PWM_SetDuty(float ch1, float ch2, float ch3, float ch4)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3); /* 读取自动重装载寄存器 ARR */

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t)(ch1 * (float)arr)); /* 设置比较值 */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t)(ch2 * (float)arr)); /* 设置比较值 */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (uint32_t)(ch3 * (float)arr)); /* 设置比较值 */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, (uint32_t)(ch4 * (float)arr)); /* 设置比较值 */
}
