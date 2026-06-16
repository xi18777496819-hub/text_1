/* gpio.c */
#include "gpio.h" /* 包含本模块头文件 */
#include "stm32f1xx_hal.h" /* 包含 HAL 库头文件 */

/* 注意：CubeMX 通常会生成 MX_GPIO_Init，本文件提供额外的通用引脚初始化函数 */
void APP_GPIO_Init(void)
{
    /* 如果需要可在此处添加额外 GPIO 初始化代码，例如配置输出推挽、上拉下拉等 */
    /* 当前项目使用 TIM PWM 输出，定时器通道的 GPIO 通常在 CubeMX 生成的文件中已配置 */
}
