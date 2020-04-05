/**
 * @file kl02z_it.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "MKL02Z4.h" // Device header
#include "kl02z_uart.h"
#include "stdio.h"
#include "share.h"

char UARTRXBuf[UART_RX_BUF_SIZE];
volatile unsigned short RXBuf_pos = 0;
volatile unsigned char esc_flag = 0;

/**
  * @brief  This function handles UART0 exception.
  * @param  None
  * @retval None
  */
void UART0_IRQHandler(void)
{
    byte_t rxchar = 0;
    while (UART0_S1 & UART0_S1_RDRF_MASK) //
    {
        rxchar = UART0_D;

        if (ESC == rxchar)
        {
            esc_flag = !esc_flag;
            RX_INI_POS();
        }
        else
        {
            if (BS == rxchar)
            {
                if (RXBuf_pos != 0)
                {
                    RX_ADVANCE_PIDX(RXBuf_pos);
                }
                UART_SendChar(BS);
                UART_SendChar(SPACE);
                UART_SendChar(BS);
            }
            else
            {
                UART_SendChar(rxchar);
                RX_PUT_CHAR(rxchar);
                RX_ADVANCE_IDX(RXBuf_pos);
                if (LF == rxchar)
                {
                    RX_INI_POS();
                    if (TRUE == esc_flag)
                    {
                        uart_cmd();
                    }
                }
            }
        }
    }
}

int fputc(int ch, FILE *f)
{
    UART_SendChar((u8)ch);
    while (UART_GetFlagStatus(UART_Flag_TC) != RESET);
    return ch;
}
