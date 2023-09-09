/*
 * SPDX-FileCopyrightText: 2023 Dreamcmi
 *
 * SPDX-License-Identifier: Apache-2.0
 */


/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/*  HPM example includes. */
#include <stdio.h>
#include "board.h"
#include "hpm_gpio_drv.h"

/* PikaPython includes */
#include "pikaScript.h"

#define PIKA_TASK_PRIORITIES (configMAX_PRIORITIES - 2)
#define PIKA_TASK_STACK_SIZE (4096U)

static void pika_task(void *pvParameters)
{
    (void)pvParameters;
    printf("[SYSTEM]enter pika task\n");
    //pikaScriptInit();
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(pikaMain, pikaModules_py_a);
    pikaScriptShell(pikaMain);
    obj_deinit(pikaMain);
    printf("[SYSTEM]leave pika task\n");
    vTaskDelete(NULL);
}

int main(void)
{
    board_init();
    board_init_gpio_pins();
#ifdef BOARD_LED_GPIO_CTRL
    gpio_set_pin_output(BOARD_LED_GPIO_CTRL, BOARD_LED_GPIO_INDEX, BOARD_LED_GPIO_PIN);
    gpio_write_pin(BOARD_LED_GPIO_CTRL, BOARD_LED_GPIO_INDEX, BOARD_LED_GPIO_PIN, BOARD_LED_ON_LEVEL);
#endif

    if (xTaskCreate(pika_task, "pika_task", PIKA_TASK_STACK_SIZE, NULL, PIKA_TASK_PRIORITIES, NULL) != pdPASS) {
        printf("pikapython task creation failed!.\n");
    }
    vTaskStartScheduler();
    printf("FreeRTOS return!!!\n");
    return 0;
}
