/* tim_pwm.c */ /* 文件：TIM PWM 驱动实现 */
#include "tim_pwm.h" /* 包含本模块头文件，声明外部接口 */
#include "stm32f1xx_hal.h" /* 包含 HAL 库以获取 TIM 相关类型与宏定义 */
#include "main.h" /* 包含项目主头以获取可能的外部定义 */

/* 定义 TIM3 句柄，作为本模块私有符号（局部定义） */
TIM_HandleTypeDef htim3; /* TIM3 句柄，用于配置与操作 TIM3 */

/* TIM PWM 初始化函数实现：配置 TIM3 基本参数并配置各通道为 PWM 模式 */
void TIM_PWM_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0}; /* 输出比较结构体，初始化为 0 */

    /* 配置定时器基本参数：预分频、计数模式、周期等 */
    htim3.Instance = TIM3; /* 指定使用 TIM3 外设 */
    htim3.Init.Prescaler = 72 - 1; /* 预分频：72 分频，假设系统时钟 72MHz */
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP; /* 向上计数模式 */
    htim3.Init.Period = 1000 - 1;  /* 自动重装载寄存器 ARR，决定 PWM 频率 */
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; /* 时钟分频 */
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; /* 关闭 ARR 预装载 */

    HAL_TIM_PWM_Init(&htim3); /* 调用 HAL 初始化 TIM PWM 基础功能 */

    /* 配置输出比较（OC）参数，设置为 PWM 模式，初始脉冲为 0 */
    sConfigOC.OCMode = TIM_OCMODE_PWM1; /* PWM 模式1 */
    sConfigOC.Pulse = 0; /* 初始比较寄存器 CCR 设置为 0（占空比 0%） */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH; /* 输出极性：高电平有效 */
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; /* 关闭快速模式 */

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1); /* 配置通道1 */
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2); /* 配置通道2 */
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3); /* 配置通道3 */
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4); /* 配置通道4 */

    /* 启动各 PWM 通道输出 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); /* 启动通道1 PWM 输出 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); /* 启动通道2 PWM 输出 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); /* 启动通道3 PWM 输出 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); /* 启动通道4 PWM 输出 */
}

/* 设置四路占空比实现，参数为 0.0f ~ 1.0f 范围（浮点） */
void TIM_PWM_SetDuty(float ch1, float ch2, float ch3, float ch4)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3); /* 读取 ARR 值以换算 CCR */

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t)(ch1 * (float)arr)); /* 设置 CCR1 */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t)(ch2 * (float)arr)); /* 设置 CCR2 */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (uint32_t)(ch3 * (float)arr)); /* 设置 CCR3 */
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, (uint32_t)(ch4 * (float)arr)); /* 设置 CCR4 */
}
