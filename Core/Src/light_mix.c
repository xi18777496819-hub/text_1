/* light_mix.c */ /* 文件：混光算法实现（查表 + 插值） */
#include "light_mix.h" /* 包含模块头文件，声明接口和类型 */
#include <stddef.h> /* 提供 size_t 定义 */

/* 查表结构：每项包含色温和对应四路占空比 */
typedef struct {
    uint16_t cct;   /* 色温值，单位 Kelvin */
    LightPWM_t pwm; /* 对应的四路占空比（0~1） */
} CCT_Entry_t; /* 定义查表条目类型 */

/* 经验查找表：稀疏点，可按需校准或扩展为更密集的表 */
static const CCT_Entry_t cct_table[] = {
    {2700, {0.05f, 0.95f, 0.08f, 0.00f}}, /* 2700K：低色温，暖白占主导 */
    {3000, {0.15f, 0.85f, 0.07f, 0.01f}}, /* 3000K：较暖 */
    {4000, {0.45f, 0.55f, 0.05f, 0.02f}}, /* 4000K：中性白 */
    {5000, {0.70f, 0.30f, 0.03f, 0.04f}}, /* 5000K：偏冷 */
    {6500, {0.90f, 0.10f, 0.01f, 0.06f}}, /* 6500K：高色温，冷白占主导 */
};

/* 表长度（项数） */
static const size_t cct_table_size = sizeof(cct_table) / sizeof(cct_table[0]);

/* 初始化接口保留用于将来校准或动态加载校正参数 */
void LightMix_Init(void)
{
    /* 当前实现无需运行时初始化，保留空实现以便后续扩展 */
}

/* 在两个表点之间进行线性插值并返回插值结果 */
static LightPWM_t interpolate(uint16_t cct)
{
    /* 如果色温不在表范围内，直接返回表端点，避免越界 */
    if (cct <= cct_table[0].cct) return cct_table[0].pwm; /* 低于最小值返回最小项 */
    if (cct >= cct_table[cct_table_size - 1].cct) return cct_table[cct_table_size - 1].pwm; /* 高于最大值返回最大项 */

    /* 遍历查找相邻两项并线性插值 */
    for (size_t i = 0; i < cct_table_size - 1; ++i) {
        if (cct >= cct_table[i].cct && cct <= cct_table[i + 1].cct) {
            float span = (float)(cct_table[i + 1].cct - cct_table[i].cct); /* 区间宽度 */
            float ratio = (span > 0.0f) ? ((float)(cct - cct_table[i].cct) / span) : 0.0f; /* 相对位置 */
            LightPWM_t a = cct_table[i].pwm; /* 左端点 */
            LightPWM_t b = cct_table[i + 1].pwm; /* 右端点 */
            LightPWM_t out; /* 输出占空比 */
            out.cold  = a.cold  + ratio * (b.cold  - a.cold);  /* 冷白插值 */
            out.warm  = a.warm  + ratio * (b.warm  - a.warm);  /* 暖白插值 */
            out.red   = a.red   + ratio * (b.red   - a.red);   /* 红色插值 */
            out.green = a.green + ratio * (b.green - a.green); /* 绿色插值 */
            return out; /* 返回插值结果 */
        }
    }

    /* 保险返回：若未找到则返回中间表项 */
    return cct_table[cct_table_size / 2].pwm;
}

/* 公共接口：根据色温和亮度计算四路占空比并返回 */
LightPWM_t LightMix_Calc(uint16_t cct, float brightness)
{
    LightPWM_t pwm = interpolate(cct); /* 首先通过查表插值得到基准占空比 */

    /* 限幅亮度参数，保证在 0~1 范围 */
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;

    /* 按亮度缩放各通道占空比 */
    pwm.cold  *= brightness; /* 冷白按比例缩放 */
    pwm.warm  *= brightness; /* 暖白按比例缩放 */
    pwm.red   *= brightness; /* 红色按比例缩放 */
    pwm.green *= brightness; /* 绿色按比例缩放 */

    /* 裁剪到合法区间 0~1，逐个通道处理以保证数值安全 */
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

    return pwm; /* 返回最终四路占空比 */
}

