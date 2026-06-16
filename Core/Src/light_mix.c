/* light_mix.c */
#include "light_mix.h" /* 本模块头文件 */
#include <stddef.h>

/* 查表结构：色温对应经验占空比（0~1） */
typedef struct {
    uint16_t cct;    /* 色温，单位 Kelvin */
    LightPWM_t pwm;  /* 对应四路占空比（0~1） */
} CCT_Entry_t;

/* 经验查找表：稀疏点，可按需校准或扩展 */
static const CCT_Entry_t cct_table[] = {
    {2700, {0.05f, 0.95f, 0.08f, 0.00f}},
    {3000, {0.15f, 0.85f, 0.07f, 0.01f}},
    {4000, {0.45f, 0.55f, 0.05f, 0.02f}},
    {5000, {0.70f, 0.30f, 0.03f, 0.04f}},
    {6500, {0.90f, 0.10f, 0.01f, 0.06f}},
};

/* 表长度 */
static const size_t cct_table_size = sizeof(cct_table) / sizeof(cct_table[0]);

/* 初始化接口保留用于将来校准或加载校正系数 */
void LightMix_Init(void)
{
    /* 当前实现不需要运行时初始化，保留占位 */
}

/* 在两个表点之间进行线性插值 */
static LightPWM_t interpolate(uint16_t cct)
{
    /* 边界处理：低于表最低项或高于表最高项时直接返回端点 */
    if (cct <= cct_table[0].cct) return cct_table[0].pwm;
    if (cct >= cct_table[cct_table_size - 1].cct) return cct_table[cct_table_size - 1].pwm;

    /* 在表中查找相邻点并插值 */
    for (size_t i = 0; i < cct_table_size - 1; ++i) {
        if (cct >= cct_table[i].cct && cct <= cct_table[i+1].cct) {
            float span = (float)(cct_table[i+1].cct - cct_table[i].cct);
            float ratio = (span > 0.0f) ? ((float)(cct - cct_table[i].cct) / span) : 0.0f;
            LightPWM_t a = cct_table[i].pwm;
            LightPWM_t b = cct_table[i+1].pwm;
            LightPWM_t out;
            out.cold = a.cold + ratio * (b.cold - a.cold);
            out.warm = a.warm + ratio * (b.warm - a.warm);
            out.red  = a.red  + ratio * (b.red  - a.red);
            out.green= a.green+ ratio * (b.green- a.green);
            return out;
        }
    }

    /* 默认返回中间项（保险） */
    return cct_table[cct_table_size/2].pwm;
}

/* 计算色温到占空比：先插值，再按总亮度缩放并裁剪 */
LightPWM_t LightMix_Calc(uint16_t cct, float brightness)
{
    LightPWM_t pwm = interpolate(cct); /* 先通过查表插值获得基准占空比 */

    /* 亮度限制 0~1 */
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;

    /* 直接按亮度缩放占空比 */
    pwm.cold  *= brightness;
    pwm.warm  *= brightness;
    pwm.red   *= brightness;
    pwm.green *= brightness;

    /* 简单裁剪，确保每路在 0~1 范围 */
    if (pwm.cold < 0.0f)
        pwm.cold = 0.0f;
    if (pwm.cold > 1.0f)
        pwm.cold = 1.0f;

    if (pwm.warm < 0.0f)
        pwm.warm = 0.0f;
    if (pwm.warm > 1.0f)
        pwm.warm = 1.0f;

    if (pwm.red < 0.0f)
        pwm.red = 0.0f;
    if (pwm.red > 1.0f)
        pwm.red = 1.0f;

    if (pwm.green < 0.0f)
        pwm.green = 0.0f;
    if (pwm.green > 1.0f)
        pwm.green = 1.0f;

    return pwm;
}

