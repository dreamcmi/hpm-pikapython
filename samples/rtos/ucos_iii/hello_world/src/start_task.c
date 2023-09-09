/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "start_task.h"
#include "user_tasks.h"

OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;

    CPU_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN
    OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif

    CPU_CRITICAL_ENTER();

    OSTaskCreate((OS_TCB *)&Led0TaskTCB,
        (CPU_CHAR *)"led0 task",
        (OS_TASK_PTR)led0_task,
        (void *)0,
        (OS_PRIO)LED0_TASK_PRIO,
        (CPU_STK *)&LED0_TASK_STK[0],
        (CPU_STK_SIZE)LED0_STK_SIZE / 10,
        (CPU_STK_SIZE)LED0_STK_SIZE,
        (OS_MSG_QTY)0,
        (OS_TICK)0,
        (void *)0,
        (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR *)&err);

    OSTaskCreate((OS_TCB *)&PrintTaskTCB,
        (CPU_CHAR *)"print task",
        (OS_TASK_PTR)print_task,
        (void *)0,
        (OS_PRIO)PRINT_TASK_PRIO,
        (CPU_STK *)&PRINT_TASK_STK[0],
        (CPU_STK_SIZE)PRINT_STK_SIZE / 10,
        (CPU_STK_SIZE)PRINT_STK_SIZE,
        (OS_MSG_QTY)0,
        (OS_TICK)0,
        (void *)0,
        (OS_OPT)OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR *)&err);

    OSTaskSuspend((OS_TCB *)&StartTaskTCB, &err);
    CPU_CRITICAL_EXIT();
}