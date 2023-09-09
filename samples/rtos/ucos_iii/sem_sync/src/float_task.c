/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "float_task.h"
#include "led_task.h"
#include "task.h"

OS_TCB FloatTaskTCB;
CPU_STK FLOAT_TASK_STK[FLOAT_STK_SIZE];

void float_task(void *p_arg)
{
    OS_ERR err;
    while (1) {
        printf("float task wait Mutex Sem to Sync.\r\n");
        OSSemPend(&SYNC_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err);
        OSTimeDlyHMSM(0, 0, 0, 100, OS_OPT_TIME_HMSM_STRICT, &err);
        printf("float task get sem.\r\n");
    }
}