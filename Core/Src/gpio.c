
/* gpio.c */ /* 文件：通用 GPIO 初始化实现（业务扩展） */
#include "gpio.h" /* 包含本模块头文件，声明 APP_GPIO_Init */
#include "stm32f1xx_hal.h" /* 包含 HAL 库头文件，提供 GPIO API */

/* 注意：CubeMX 通常在 MX_GPIO_Init 中生成主要 GPIO 配置，本文件提供额外的应用级 GPIO 初始化入口 */
void APP_GPIO_Init(void)
{
    /* 在此处实现额外的 GPIO 配置，例如按键、指示灯或外部使能脚的初始化 */
    /* 示例：如果需要可以调用 HAL_GPIO_Init 配置某个引脚为推挽输出 */
    /* 当前项目的 PWM 引脚由 CubeMX 配置，这里保持空实现作为占位 */
}
