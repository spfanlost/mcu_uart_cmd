

#ifndef __kl02z_uart_H
#define __kl02z_uart_H

#include "MKL02Z4.h" // Device header
#include "share.h"

#define UART_RX_BUF_SIZE 128

#define RX_NEXT_IDX(cur_idx)        ((cur_idx + 1) % UART_RX_BUF_SIZE)
#define RX_PRE_IDX(cur_idx)         ((cur_idx - 1) % UART_RX_BUF_SIZE)
#define RX_ADVANCE_IDX(p)           do {p = RX_NEXT_IDX(p);} while (0)
#define RX_ADVANCE_PIDX(p)          do {p = RX_PRE_IDX(p);} while (0)
#define RX_PUT_CHAR(c)              do {UARTRXBuf[RXBuf_pos] = c;} while (0)
#define RX_GET_CHAR(c)              do {c = UARTRXBuf[RXBuf_pos];} while (0)
#define RX_INI_POS()                do {RXBuf_pos = 0;} while (0)

typedef enum
{
    UART_IT_TXE = UART0_C2_TIE_MASK,   /*!< transmit buffer empty interrupt */
    UART_IT_TC = UART0_C2_TCIE_MASK,   /*!< transmit complete interrupt */
    UART_IT_RXNE = UART0_C2_RIE_MASK,  /*!< receive buffer full interrupt */
    UART_IT_IDLE = UART0_C2_ILIE_MASK, /*!< idle line interrupt */
    UART_IT_ORE = UART0_C3_ORIE_MASK,  /*!< receive overrun interrupt */
    UART_IT_NERR = UART0_C3_NEIE_MASK, /*!< noise error interrupt */
    UART_IT_ERR = UART0_C3_FEIE_MASK,  /*!< framing error interrupt */
    UART_IT_PE = UART0_C3_FEIE_MASK,   /*!< parity error interrupt */
} UART_InterruptType;

typedef enum
{
    UART_Flag_PF = 0, /*!< Parity error flag */
    UART_Flag_FE,     /*!< Framing error flag */
    UART_Flag_NF,     /*!< Noise flag */
    UART_Flag_ORE,    /*!< Receive overrun */
    UART_Flag_IDLE,   /*!< Idle line flag */
    UART_Flag_RDRF,   /*!< Receive data register full flag */
    UART_Flag_TC,     /*!< Transmission complete flag */
    UART_Flag_TDRE,   /*!< Transmit data register flag */

    UART_Flag_RAF,     /*!< Receiver active flag */
    UART_Flag_LBKDE,   /*!< LIN break detection enable */
    UART_Flag_BRK13,   /*!< Break character generation length */
    UART_Flag_RWUID,   /*!< Receive wake up idle detect */
    UART_Flag_RXINV,   /*!< Receive data inversion */
    UART_Flag_Rev1,    /*!< Reserved */
    UART_Flag_RXEDGIF, /*!< RxD pin active edge interrupt flag */
    UART_Flag_LBKDIF,  /*!< LIN break detect interrupt flag */
} UART_FlagType;

extern char UARTRXBuf[UART_RX_BUF_SIZE];
extern volatile unsigned short  RXBuf_pos;
extern volatile unsigned char esc_flag;

void UART_Init(uint32_t UART_BaudRate);
void UART_ITConfig(UART_InterruptType UART_IT, FunctionalState NewState);
FlagStatus UART_GetFlagStatus(UART_FlagType UART_FLAG);
void UART_SendChar(unsigned char c);
void UART_SendStr(char *buff);

#endif
