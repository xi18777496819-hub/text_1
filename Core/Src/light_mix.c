/* light_mix.c */
#include "light_mix.h" /* 本模块头文件 */
#include <math.h> /* 使用数学函数 */

/* 简单线性插值算法：根据色温计算冷白/暖白比例，辅以红绿微调 */
void LightMix_Init(void)
{
    /* 当前实现无需初始化，保留入口以便后续校准 */
}

/* 将输入色温映射到 0~1 的冷/暖比例并乘以总亮度 */
LightPWM_t LightMix_Calc(uint16_t cct, float brightness)
{
    LightPWM_t out; /* 输出结构体 */

    /* 限制输入范围到 3000~6500 */
    if (cct < 3000) cct = 3000;
    if (cct > 6500) cct = 6500;

    /* 计算相对位置 0~1，0 对应 3000K（暖），1 对应 6500K（冷） */
    float t = (float)(cct - 3000) / (float)(6500 - 3000);

    /* 冷白与暖白线性插值，冷白在高色温占比增加 */
    out.cold = (0.2f + 0.8f * t) * brightness; /* 冷白基准 0.2~1.0 */
    out.warm = (0.8f - 0.8f * t) * brightness; /* 暖白基准 0.8~0.0 */

    /* 红绿小幅度随色温变化，低色温增加红色，高色温增加绿/冷感 */
    out.red = (0.05f + 0.10f * (1.0f - t)) * brightness; /* 红色 0.05~0.15 */
    out.green = (0.02f + 0.08f * t) * brightness; /* 绿色 0.02~0.10 */

    /* 归一化确保总亮度不超过亮度参数（简单裁剪） */
    float sum = out.cold + out.warm + out.red + out.green;
    if (sum > brightness && sum > 0.0f)
    {
        float scale = brightness / sum;
        out.cold *= scale;
        out.warm *= scale;
        out.red *= scale;
        out.green *= scale;
    }

    return out; /* 返回计算得到的占空比 */
}
