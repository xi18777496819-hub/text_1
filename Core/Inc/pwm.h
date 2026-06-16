#ifndef __PWM_H
#define __PWM_H

#include <stdint.h>

void PWM_Init(void);
void PWM_SetValue(float ch1, float ch2, float ch3, float ch4);

#endif // __PWM_H
