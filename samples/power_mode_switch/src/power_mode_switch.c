/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "hpm_uart_drv.h"
#include "low_power.h"

char uart_get_char(void);

void show_menu(void);

volatile bool has_data;
volatile uint8_t byte_read;

int main(void)
{
    has_data = false;
    byte_read = 0;

    board_init();

    uart_enable_irq(HPM_PUART, uart_intr_rx_data_avail_or_timeout);
    intc_m_enable_irq_with_priority(IRQn_PUART, 1);
    sysctl_enable_cpu0_wakeup_source_with_irq(HPM_SYSCTL, IRQn_PUART);

    prepare_soc_low_power();
    show_menu();

    while (true) {
        char ch = uart_get_char();
        if (ch == 'w') {
            continue;
        }
        switch (ch) {
        case '1':
            enter_wait_mode();
            printf("Waked up from the wait mode\r\n");
            break;
        case '2':
            enter_stop_mode();
            printf("Waked up from the stop mode\r\n");
            break;
        case '3':
            enter_standby_mode();
            printf("Waked up from the standby mode\r\n");
            break;
        case '4':
            enter_shutdown_mode();
            printf("Waked up from the shutdown mode\r\n");
            break;
        default:
            break;
        }
        show_menu();
    }
}

void show_menu(void)
{
    const char menu_str[] =
        "Power mode switch demo:\r\n"
        "1 - Enter wait mode\r\n"
        "2 - Enter stop mode\r\n"
        "3 - Enter standby mode\r\n"
        "4 - Enter shutdown mode\r\n";
    printf(menu_str);
}

void puart_isr(void)
{
    if (uart_check_status(HPM_PUART, uart_stat_data_ready)) {
        has_data = true;
        byte_read = uart_read_byte(HPM_PUART);
    }
}

char uart_get_char(void)
{
    while (!has_data) {
    }
    has_data = false;
    return byte_read;
}

SDK_DECLARE_EXT_ISR_M(IRQn_PUART, puart_isr)

