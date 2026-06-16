#include "main.h"
#include "../Inc/light_mix.h"
#include "../Inc/pwm.h"

/*
 * 示例说明：
 * - 请确保工程中已由 CubeMX 或手工代码生成并初始化了 htim3
 *  （定时器的 PWM 通道 1..4），以及 SystemClock_Config()/MX_TIM3_Init()。
 * - 将下面循环逻辑合并到你现有的 main.c 中，或替换为你自己的集成方式。
 */

extern void SystemClock_Config(void);
// extern void MX_TIM3_Init(void); // 如果工程有该函数则调用

int example_main_light(void)
{
    HAL_Init();
    SystemClock_Config();

    // 如果工程生成了 MX_TIM3_Init，请在此调用以配置定时器
    // MX_TIM3_Init();

    PWM_Init();

    Light_SetMode(MODE_DAY);
    Light_SetBrightness(0.9f);

    while (1)
    {
        PWM_Output_t pwm = Light_GetPWM();

        PWM_SetValue(pwm.ch1, pwm.ch2, pwm.ch3, pwm.ch4);

        HAL_Delay(50);
    }

    return 0;
}
