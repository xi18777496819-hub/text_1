
/* gpio.c */ /* 文件：通用 GPIO 初始化实现（业务扩展） */
#include "gpio.h" /* 包含本模块头文件，声明 APP_GPIO_Init */
#include "stm32f1xx_hal.h" /* 包含 HAL 库头文件，提供 GPIO API */

/* 注意：CubeMX 通常在 MX_GPIO_Init 中生成主要 GPIO 配置，本文件提供额外的应用级 GPIO 初始化入口 */
void APP_GPIO_Init(void)
{
    /* 在此处实现额外的 GPIO 配置，例如按键、指示灯或外部使能脚的初始化 */
    /* 本项目示例：为 TIM3 的 4 路 PWM 配置 GPIO
       默认映射（STM32F103）：TIM3_CH1 = PA6, TIM3_CH2 = PA7, TIM3_CH3 = PB0, TIM3_CH4 = PB1
       为保证 PB0/PB1 可作为 TIM3 输出，需要对 TIM3 做部分重映射（AFIO）。
       下面代码开启相应时钟、执行重映射并把引脚设为复用推挽输出（AF_PP）。 */

    GPIO_InitTypeDef GPIO_InitStruct = {0}; /* GPIO 配置结构体 */

    /* 使能 GPIOA、GPIOB、AFIO 时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    /* 对 TIM3 做部分重映射以将 CH3/CH4 映射到 PB0/PB1（F103 系列） */
    __HAL_AFIO_REMAP_TIM3_PARTIAL(); /* 注意：若已由 CubeMX 设置，此调用可能重复无害 */

    /* 配置 PA6 (TIM3_CH1) 与 PA7 (TIM3_CH2) 为复用推挽输出 */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7; /* PA6, PA7 */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        /* 复用推挽 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  /* 高速 */
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        /* 初始化 PA6/PA7 */

    /* 配置 PB0 (TIM3_CH3) 与 PB1 (TIM3_CH4) 为复用推挽输出 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1; /* PB0, PB1 */
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        /* 复用推挽 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  /* 高速 */
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);        /* 初始化 PB0/PB1 */

    /* 注意：如果你的硬件使用了其它引脚，请告诉我具体引脚，我会调整此处配置。 */
}
