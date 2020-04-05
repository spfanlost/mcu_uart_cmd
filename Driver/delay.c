/**
 * @file delay.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "MKL02Z4.h"
#include "delay.h"

u8 times = 0;
void SysTick_Handler()
{
    times = 1;
}
void delay_ms1(u16 nms) //348
{
    SysTick_Config(((uint32_t)(SystemCoreClock / 1000)) * nms);
    while (!times)
        ;
    times = 0;
    SysTick->CTRL = 0x00;
}

void delay_ms(u16 nms)
{
    u8 i;
    for (i = 0; i < 100; i++)
    {
        if ((nms - 348) >= 0)
        {
            delay_ms1(348);
            nms = nms - 348;
        }
        else
        {
            delay_ms1(nms);
            break;
        }
    }
}
void delay_us(u32 nus)
{
    SysTick_Config((SystemCoreClock / 1000000) * nus);
    while (!times);
    times = 0;
    SysTick->CTRL = 0x00;
}
