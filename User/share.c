/**
 * @file share.c
 * @author imyumeng (imyumeng@qq.com)
 * @brief 
 * @version 0.1
 * @date 2019-08-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "share.h"
#include "kl02z_uart.h"
#include "systeminfo.h"

byte_t uart_get_var(char *str, byte_t *pos, dword_t *para, byte_t idx);
static void uart_cmd_help(char *str, byte_t *pos);
static void uart_cmd_info(char *str, byte_t *pos);
static void uart_cmd_dump(char *str, byte_t *pos);
static void uart_cmd_regs(char *str, byte_t *pos);

const uart_cmd_t g_uartcmd_list[] = {
    {uart_cmd_help, 'h', 'e', 'l', 'p', '\0', '\0', '\0', '\0', 4, 0},
    {uart_cmd_dump, 'd', 'u', 'm', 'p', '\0', '\0', '\0', '\0', 4, 3},
    {uart_cmd_regs, 'r', 'e', 'g', 's', '\0', '\0', '\0', '\0', 4, 2},
    {uart_cmd_info, 'i', 'n', 'f', 'o', '\0', '\0', '\0', '\0', 4, 0},
};

static void uart_cmd_help(char *str, byte_t *pos)
{
    LOG(LOG_INFO, "Use ESC change mode.\r");
    LOG(LOG_INFO, "Command list(hex data):\r");
    LOG(LOG_INFO, "1.dump <address> <rows> <columns> \r");
    LOG(LOG_INFO, "2.regs <address> <value> \r");
    LOG(LOG_INFO, "3.info SystemInfo \r");
}

static void uart_cmd_info(char *str, byte_t *pos)
{
    SystemInfo();
}

static void uart_cmd_dump(char *str, byte_t *pos)
{
    dword_t *ptr = NULL;
    dword_t rows = 0, columns = 0;
    dword_t mem_addr = 0, rows_cnt = 0, columns_cnt = 0;
    if (!uart_get_var(str, pos, &mem_addr, CMD_VAL1))
        return;
    if (!uart_get_var(str, pos, &rows_cnt, CMD_VAL2))
        return;
    if (!uart_get_var(str, pos, &columns_cnt, CMD_VAL3))
        return;
    ptr = (dword_t *)mem_addr;
    LOG(LOG_INFO, "addr(0x%X) rows(0x%X) columns(0x%X)\r", mem_addr, rows_cnt, columns_cnt);
    for (rows = 0; rows < rows_cnt; rows++)
    {
        LOG(LOG_INFO, "%08X: ", mem_addr + rows * 4 * columns_cnt);
        for (columns = 0; columns < columns_cnt; columns++)
        {
            LOG(LOG_INFO, "%08X ", ptr[columns]);
        }
        LOG(LOG_INFO, "\r");
        ptr += columns_cnt;
    }
}

static void uart_cmd_regs(char *str, byte_t *pos)
{
    dword_t reg_addr;
    dword_t reg_value;
    if (FALSE == uart_get_var(str, pos, &reg_addr, CMD_VAL1))
        return;
    if (FALSE == uart_get_var(str, pos, &reg_value, CMD_VAL2))
        return;

    REG_GET(reg_addr) = reg_value;
    if (reg_value != REG_GET(reg_addr))
        LOG(LOG_ERROR, "adrr:0x%x 0x%x != 0x%x\r", reg_addr, REG_GET(reg_addr), reg_value);
    else
        LOG(LOG_INFO, "reg set ok\r");
    return;
}

byte_t char_to_val(char c)
{
    byte_t val = 0;
    if ('0' <= c && c <= '9')
        val = c - '0';
    else if ('A' <= c && c <= 'F')
        val = 0xa + c - 'A';
    else if ('a' <= c && c <= 'f')
        val = 0xa + c - 'a';
    return val;
}

byte_t uart_get_var(char *str, byte_t *pos, dword_t *para, byte_t idx)
{
    byte_t paralen = 0, num = 0;
    if (CMD_INIT < idx && CMD_NUM > idx)
    {
        if (0 == pos[idx])
            return 0;
        paralen = pos[idx] - pos[idx - 1] - 1;
        if (paralen > (sizeof(dword_t) << 1))
            return 0;
        for (num = 0; num < paralen; num++)
        {
            *para <<= 4;
            *para |= char_to_val(str[pos[idx - 1] + 1 + num]);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

byte_t check_cmd(char *ptr, byte_t *val_pos, byte_t *val_num)
{
    byte_t idx = 0;
    if (RXBuf_pos != 0)
        return FALSE;
    while (idx < UART_RX_BUF_SIZE)
    {
        if (' ' == ptr[idx] && *val_num < CMD_NUM)
        {
            val_pos[(*val_num)++] = idx;
        }
        else if ('\r' == ptr[idx] /*&& '\n' == ptr[idx+1]*/)
        {
            if (idx > 0 && *val_num < CMD_NUM)
            {
                val_pos[(*val_num)++] = idx;
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        idx++;
    }
    return FALSE;
}

void uart_cmd(void)
{
    byte_t index = 0;
    byte_t varanum = 0;
    char *ptr = UARTRXBuf;
    byte_t pos[CMD_NUM] = {0};

    if (check_cmd(ptr, pos, &varanum))
    {
        for (index = 0; index < ARRAY_SIZE(g_uartcmd_list); index++)
        {
            if ((g_uartcmd_list[index].paranum == varanum - 1) &&
                (strncmp(ptr, g_uartcmd_list[index].cmd, g_uartcmd_list[index].cmdlen) == 0))
            {
                if (NULL != g_uartcmd_list[index].pfn)
                {
                    g_uartcmd_list[index].pfn(ptr, pos);
                    memset((void *)UARTRXBuf, '\0', UART_RX_BUF_SIZE);
                }
            }
        }
    }
    else
    {
        memset((void *)UARTRXBuf, '\0', UART_RX_BUF_SIZE);
        LOG(LOG_INFO, "ISR>");
    }
}
