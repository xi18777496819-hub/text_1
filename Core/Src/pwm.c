#include "../Inc/pwm.h"
#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim3; // 请在工程中定义并初始化 htim3

static inline float clampf(float v, float a, float b)
{
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

void PWM_SetValue(float ch1, float ch2, float ch3, float ch4)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3);

    uint32_t c1 = (uint32_t)(clampf(ch1, 0.0f, 1.0f) * (float)arr);
    uint32_t c2 = (uint32_t)(clampf(ch2, 0.0f, 1.0f) * (float)arr);
    uint32_t c3 = (uint32_t)(clampf(ch3, 0.0f, 1.0f) * (float)arr);
    uint32_t c4 = (uint32_t)(clampf(ch4, 0.0f, 1.0f) * (float)arr);

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, c1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, c2);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, c3);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, c4);
}

void PWM_Init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}
