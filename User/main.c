/**
 * @file main.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "MKL02Z4.h" // Device header
#include "delay.h"
#include "kl02z_uart.h"
#include "kl02z_tpm.h"
#include "systeminfo.h"

int main(void)
{
    int i = 0;
    //int cnt = 0;
    SystemCoreClockUpdate();

    UART_Init(115200);                   //N-8-1
    UART_ITConfig(UART_IT_RXNE, ENABLE); //
    NVIC_Init(UART0_IRQn, 1);            //

    PORTB->PCR[6] = PORT_PCR_MUX(2);
    PORTB->PCR[7] = PORT_PCR_MUX(2);
    PORTB->PCR[10] = PORT_PCR_MUX(2);
    TPM_PWM_Init(TPM1, 1, 1000, 0);
    TPM_PWM_Init(TPM1, 0, 1000, 0);
    TPM_PWM_Init(TPM0, 1, 1000, 0);
    delay_ms(1);
    LOG(LOG_INFO, "__FILENAME__:%s\n", __FILENAME__);
    LOG(LOG_INFO, "__FILE__:%s\n", __FILE__);
    SystemInfo(); //
    while (1)
    {
        for (i = 0; i < 101; i++)
        {
            TPM_PWM_Duty(TPM1, 1, i);
            delay_ms(10);
        }
        for (i = 100; i >= 0; i--)
        {
            TPM_PWM_Duty(TPM1, 1, i);
            delay_ms(10);
        }
        for (i = 0; i < 101; i++)
        {
            TPM_PWM_Duty(TPM0, 1, i);
            delay_ms(10);
        }
        for (i = 100; i >= 0; i--)
        {
            TPM_PWM_Duty(TPM0, 1, i);
            delay_ms(10);
        }
        for (i = 0; i < 101; i++)
        {
            TPM_PWM_Duty(TPM1, 0, i);
            delay_ms(10);
        }
        for (i = 100; i >= 0; i--)
        {
            TPM_PWM_Duty(TPM1, 0, i);
            delay_ms(10);
        }
        // fflush(stdout);
        // scanf("%d", &test_case);
    }
}
