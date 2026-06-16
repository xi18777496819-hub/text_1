#include "../Inc/light_mix.h"
#include <math.h>

typedef struct
{
    uint16_t cct;
    PWM_Output_t pwm;
} CCT_Table_t;

static const CCT_Table_t cct_table[] =
{
    {2700, {0.05f, 0.95f, 0.08f, 0.00f}},
    {3000, {0.15f, 0.85f, 0.07f, 0.01f}},
    {4000, {0.45f, 0.55f, 0.05f, 0.02f}},
    {5000, {0.70f, 0.30f, 0.03f, 0.04f}},
    {6500, {0.90f, 0.10f, 0.01f, 0.06f}},
};

static Light_Mode_t current_mode = MODE_DAY;
static uint16_t target_cct = 4000;
static float global_brightness = 1.0f; // 0.0 ~ 1.0

static inline float clampf(float v, float a, float b)
{
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

static PWM_Output_t interpolate(uint16_t cct)
{
    int size = sizeof(cct_table) / sizeof(cct_table[0]);

    if (cct <= cct_table[0].cct)
        return cct_table[0].pwm;

    if (cct >= cct_table[size - 1].cct)
        return cct_table[size - 1].pwm;

    for (int i = 0; i < size - 1; i++)
    {
        if (cct >= cct_table[i].cct && cct <= cct_table[i + 1].cct)
        {
            float ratio = (float)(cct - cct_table[i].cct) / (float)(cct_table[i + 1].cct - cct_table[i].cct);
            PWM_Output_t out;
            out.ch1 = cct_table[i].pwm.ch1 + ratio * (cct_table[i + 1].pwm.ch1 - cct_table[i].pwm.ch1);
            out.ch2 = cct_table[i].pwm.ch2 + ratio * (cct_table[i + 1].pwm.ch2 - cct_table[i].pwm.ch2);
            out.ch3 = cct_table[i].pwm.ch3 + ratio * (cct_table[i + 1].pwm.ch3 - cct_table[i].pwm.ch3);
            out.ch4 = cct_table[i].pwm.ch4 + ratio * (cct_table[i + 1].pwm.ch4 - cct_table[i].pwm.ch4);
            return out;
        }
    }

    return cct_table[size/2].pwm;
}

void Light_SetMode(Light_Mode_t mode)
{
    current_mode = mode;

    switch (mode)
    {
        case MODE_DAY:    target_cct = 6500; break;
        case MODE_RELAX:  target_cct = 4000; break;
        case MODE_NIGHT:  target_cct = 2700; break;
    }
}

void Light_SetColorTemp(uint16_t cct)
{
    target_cct = cct;
}

void Light_SetBrightness(float brightness)
{
    global_brightness = clampf(brightness, 0.0f, 1.0f);
}

PWM_Output_t Light_GetPWM(void)
{
    PWM_Output_t pwm = interpolate(target_cct);

    /* 夜间蓝光抑制 */
    if (current_mode == MODE_NIGHT)
    {
        pwm.ch4 *= 0.3f;   // 降低蓝光
        pwm.ch3 *= 1.2f;   // 提升红光
    }

    /* 亮度缩放（线性），并做简单 gamma 校正以保持感觉亮度 */
    const float gamma = 2.2f;
    PWM_Output_t out;

    out.ch1 = powf(clampf(pwm.ch1, 0.0f, 1.0f) * global_brightness, 1.0f / gamma);
    out.ch2 = powf(clampf(pwm.ch2, 0.0f, 1.0f) * global_brightness, 1.0f / gamma);
    out.ch3 = powf(clampf(pwm.ch3, 0.0f, 1.0f) * global_brightness, 1.0f / gamma);
    out.ch4 = powf(clampf(pwm.ch4, 0.0f, 1.0f) * global_brightness, 1.0f / gamma);

    /* 防止数值超限 */
    out.ch1 = clampf(out.ch1, 0.0f, 1.0f);
    out.ch2 = clampf(out.ch2, 0.0f, 1.0f);
    out.ch3 = clampf(out.ch3, 0.0f, 1.0f);
    out.ch4 = clampf(out.ch4, 0.0f, 1.0f);

    return out;
}
