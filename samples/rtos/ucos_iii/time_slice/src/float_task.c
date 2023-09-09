/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "float_task.h"
#include "led_task.h"
#include "stdint.h"
#include "board.h"

OS_TCB FloatTaskTCB;
CPU_STK FLOAT_TASK_STK[FLOAT_STK_SIZE];

void float_task(void *p_arg)
{
    uint32_t temp = 0;
    uint8_t i = 0;
    CPU_SR_ALLOC();
    static float float_num = 0.01;
    while (1) {
        float_num += 0.01f;
        CPU_CRITICAL_ENTER();
        printf("float run %d times\r\n", temp++);
        CPU_CRITICAL_EXIT();
        for (i = 0; i < 5; i++) {
            printf("float : 67890 \r\n");
            board_delay_ms(100);
        }
    }
}