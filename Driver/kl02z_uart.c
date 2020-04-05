/**
 * @file kl02z_uart.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "kl02z_uart.h"

void UART_Init(uint32_t UART_BaudRate)
{
    uint8_t temp;
    uint16_t sbr;

    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); //PLL clock
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;  //open clock
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

    PORTB->PCR[1] = PORT_PCR_MUX(0x2);
    PORTB->PCR[2] = PORT_PCR_MUX(0x2); //Tx PTB2

    //forbid send/receive
    UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
    /* Configure the uart for 8-bit mode, no parity */
    UART0->C1 = 0;

    //set baud rate
    temp = UART0_C4;
    temp = (temp & UART0_C4_OSR_MASK) + 1;
    sbr = (uint16_t)((SystemCoreClock) / (UART_BaudRate * (temp)));
    temp = UART0_BDH & ~(UART0_BDH_SBR(0x1F));

    UART0_BDH = temp | UART0_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART0_BDL = (uint8_t)(sbr & UART0_BDL_SBR_MASK);

    UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK); //enable TX RX
}

/**
* @brief  Enables or disables the specified USART interrupts.
* @param  UARTx: Select the UART or the UART peripheral. 
*   This parameter can be one of the following values:
*   UART0,UART1, UART2, 
* @param  USART_IT: specifies the UART interrupt sources to be enabled or disabled.
*   This parameter can be one of the following values:

*     @arg UART_IT_TXE:  Transmit Data Register empty interrupt
*     @arg UART_IT_TC:   Transmission complete interrupt
*     @arg UART_IT_RXNE: Receive Data register not empty interrupt
*     @arg UART_IT_IDLE: Idle line detection interrupt
*     @arg UART_IT_ORE:  receive overrun interrupt 
*     @arg UART_IT_NERR:  noise error interrupt
*     @arg UART_IT_PE:    Parity Error interrupt
*     @arg UART_IT_ERR:   framing error interrupt
* @param  NewState: new state of the specified USARTx interrupts.
*   This parameter can be: ENABLE or DISABLE.
* @retval None
*/

void UART_ITConfig(UART_InterruptType UART_IT, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        UART0->C2 |= UART_IT;
    }
    else
    {
        UART0->C2 &= ~UART_IT;
    }
}
/**
* @brief  Checks whether the specified USART flag is set or not.
* @param  UARTx: Select the USART or the UART peripheral. 
*   This parameter can be one of the following values:
*   UART0, UART1, UART2,
* @param  USART_FLAG: specifies the flag to check.
*   This parameter can be one of the following values:
*     @arg UART_Flag_PF:  Parity error flag
*     @arg UART_Flag_FE:  Framing error flag
*     @arg UART_Flag_TDRE:  Transmit data register empty flag
*     @arg UART_Flag_TC:   Transmission Complete flag
*     @arg UART_FLAG_RXNE: Receive data register not empty flag
*     @arg UART_Flag_IDLE: Idle Line detection flag
*     @arg UART_Flag_ORE:  OverRun Error flag
*     @arg UART_Flag_RAF:   Receiver active flag
*     @arg UART_Flag_LBKDE:   LIN break detection enable
*     @arg UART_Flag_BRK13:   Break character generation length
*     @arg UART_Flag_RWUID:   Receive wake up idle detect
*     @arg UART_Flag_RXINV:   Receive data inversion
*     @arg UART_Flag_Rev1:    Reserved
*     @arg UART_Flag_RXEDGIF:   RxD pin active edge interrupt flag 
*     @arg UART_Flag_LBKDIF:   LIN break detect interrupt flag
* @retval The new state of UART_FLAG (SET or RESET).
*/
FlagStatus UART_GetFlagStatus(UART_FlagType UART_FLAG)
{
    FlagStatus bitstatus = RESET;
    uint16_t u16StatusFlags = 0;
    u16StatusFlags = UART0->S2;
    u16StatusFlags = (u16StatusFlags << 8) | UART0->S1;
    if ((u16StatusFlags & (1 << UART_FLAG)) != (uint16_t)RESET)
        bitstatus = SET;
    else
        bitstatus = RESET;
    return bitstatus;
}

void UART_SendChar(unsigned char c)
{
    while (!(UART0_S1 & UART0_S1_TDRE_MASK));
    UART0_D = c;
}

void UART_SendStr(char *buff)
{
    while ((0 != *buff))
    {
        UART_SendChar(*buff++);
        while (UART_GetFlagStatus(UART_Flag_TC) != RESET);
    }
}

char UART_Getchar()
{
    while (!(UART0_S1)&UART0_S1_RDRF_MASK);
    return UART0_D;
}
