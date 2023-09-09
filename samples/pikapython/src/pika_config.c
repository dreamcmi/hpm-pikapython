/*
 * SPDX-FileCopyrightText: 2023 Dreamcmi
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "pika_config.h"

int pika_platform_putchar(char ch) {
    while (status_success != uart_send_byte(BOARD_CONSOLE_BASE, ch));
    while (status_success != uart_flush(BOARD_CONSOLE_BASE));
    return 0;
}

char pika_platform_getchar(void) {
    uint8_t buff;
    while (status_success != uart_receive_byte(BOARD_CONSOLE_BASE, &buff)) {
        vTaskDelay(1);
    }
    return (char)buff;
}

void* pika_platform_malloc(size_t size) {
    return pvPortMalloc(size);
}

void pika_platform_free(void* ptr) {
    vPortFree(ptr);
}

void pika_platform_sleep_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void pika_platform_sleep_s(uint32_t s) {
    vTaskDelay(pdMS_TO_TICKS(1000 * s));
}