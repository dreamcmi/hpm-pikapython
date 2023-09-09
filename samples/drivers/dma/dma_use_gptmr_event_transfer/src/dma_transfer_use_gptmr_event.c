/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifdef CONFIG_HAS_HPMSDK_DMAV2
#include "hpm_dmav2_drv.h"
#else
#include "hpm_dma_drv.h"
#endif
#include "hpm_gptmr_drv.h"
#include "hpm_gpio_drv.h"
#include "hpm_dmamux_drv.h"
#include "board.h"

#define APP_DMA_SRC_WIDTH            DMA_TRANSFER_WIDTH_WORD
#define APP_DMA_DST_WIDTH            DMA_TRANSFER_WIDTH_WORD
#define APP_BOARD_GPTMR              BOARD_GPTMR
#define APP_BOARD_GPTMR_CLOCK        BOARD_GPTMR_CLK_NAME
#define APP_BOARD_GPTMR_CH           BOARD_GPTMR_CHANNEL
#define APP_BOARD_GPTMR_DMA_SRC      BOARD_GPTMR_DMA_SRC
#define APP_BOARD_RELOAD_MS          (500)
#define APP_BOARD_CMP_MS             (100)

typedef struct {
    uint8_t num;
    char *num_str;
} test_number_t;

const test_number_t test_table[] = {
    {gptmr_dma_request_on_cmp0,                "*        0. use gptmr cmp0 event                                                  *\n"},
    {gptmr_dma_request_on_cmp1,                "*        1. use gptmr cmp1 event                                                  *\n"},
    {gptmr_dma_request_on_input_signal_toggle, "*        2. use gptmr input_signal_toggle event                                   *\n"},
    {gptmr_dma_request_on_reload,              "*        3. use gptmr reload event                                                *\n"},
};

uint32_t pins_toggle_status;

static void dma_transfer_config(void);
static void gptmr_config(gptmr_dma_request_event_t event);
static void show_help(void);
static void handle_gptmr_event_test(void);

void isr_dma(void)
{
    volatile hpm_stat_t stat;
    volatile uint32_t src_addr;
    stat = dma_check_transfer_status(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0);
    if (stat & DMA_CHANNEL_STATUS_TC) {

        /* in order to show the demo need to reset count, example:in gptmr_dma_request_on_cmp0 mode,the flash time of led varies with cmp */
        gptmr_channel_reset_count(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH);

        src_addr = core_local_mem_to_sys_address(HPM_CORE0, (uint32_t)&pins_toggle_status);
        dma_set_source_address(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0, src_addr);

        dma_set_transfer_size(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0, sizeof(pins_toggle_status) >> APP_DMA_SRC_WIDTH);

        dma_enable_channel(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0);
    }
}
SDK_DECLARE_EXT_ISR_M(BOARD_APP_HDMA_IRQ, isr_dma)

int main(void)
{
    /*the TOGGLE register of the LED pin set 1,it's means the leve will filp*/
    pins_toggle_status = (1 << BOARD_LED_GPIO_PIN);

    board_init();
    board_init_led_pins();
    handle_gptmr_event_test();
}

static void dma_transfer_config(void)
{
    dma_channel_config_t ch_config = {0};
    dma_disable_channel(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0);
    dma_reset(BOARD_APP_HDMA);
    dma_default_channel_config(BOARD_APP_HDMA, &ch_config);
    ch_config.src_addr = core_local_mem_to_sys_address(HPM_CORE0, (uint32_t)&pins_toggle_status);
    ch_config.dst_addr = (uint32_t)&BOARD_LED_GPIO_CTRL->DO[BOARD_LED_GPIO_INDEX].TOGGLE;
    ch_config.src_mode = DMA_HANDSHAKE_MODE_NORMAL;
    ch_config.src_width = APP_DMA_SRC_WIDTH;
    ch_config.src_addr_ctrl = DMA_ADDRESS_CONTROL_INCREMENT;
    ch_config.src_burst_size = DMA_NUM_TRANSFER_PER_BURST_1T;
    ch_config.dst_width = APP_DMA_DST_WIDTH;
    ch_config.dst_addr_ctrl = DMA_ADDRESS_CONTROL_FIXED;
    ch_config.dst_mode = DMA_HANDSHAKE_MODE_HANDSHAKE;
    ch_config.size_in_byte = sizeof(pins_toggle_status);
    if (status_success != dma_setup_channel(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0, &ch_config, false)) {
        printf(" dma setup channel failed\n");
        return;
    }
    intc_m_enable_irq_with_priority(BOARD_APP_HDMA_IRQ, 4);
}

static void gptmr_config(gptmr_dma_request_event_t event)
{
    uint32_t gptmr_freq;
    gptmr_channel_config_t config;
    uint32_t cmp;
    gptmr_stop_counter(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH);
    dma_transfer_config();
    gptmr_channel_get_default_config(APP_BOARD_GPTMR, &config);
    clock_add_to_group(APP_BOARD_GPTMR_CLOCK, 0);
    gptmr_freq = clock_get_frequency(APP_BOARD_GPTMR_CLOCK);
    config.cmp_initial_polarity_high = false;
    config.dma_request_event = event;
    config.reload = gptmr_freq / 1000 * APP_BOARD_RELOAD_MS;
    cmp = APP_BOARD_CMP_MS;
    if (cmp > APP_BOARD_RELOAD_MS) {
        cmp = APP_BOARD_RELOAD_MS;
    }
    if (event == gptmr_dma_request_on_cmp0) {
        config.enable_cmp_output = true;
        config.cmp[0] = gptmr_freq / 1000 * APP_BOARD_CMP_MS;
    } else if (event == gptmr_dma_request_on_cmp1) {
        config.enable_cmp_output = true;
        config.cmp[1] = gptmr_freq / 1000 * APP_BOARD_CMP_MS;
    } else if (event == gptmr_dma_request_on_input_signal_toggle) {
        init_gptmr_pins(APP_BOARD_GPTMR);
        config.mode = gptmr_work_mode_capture_at_both_edge;
    }
    gptmr_channel_reset_count(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH);
    gptmr_channel_config(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH, &config, false);
    dmamux_config(BOARD_APP_DMAMUX, DMAMUX_MUXCFG_HDMA_MUX0, APP_BOARD_GPTMR_DMA_SRC, true);

    dma_enable_channel(BOARD_APP_HDMA, DMAMUX_MUXCFG_HDMA_MUX0);
    gptmr_start_counter(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH);
}

static void handle_gptmr_event_test(void)
{
    show_help();

    while (true) {
        char option = getchar();
        gptmr_dma_request_event_t event = (option - '0');
        putchar(option);
        putchar('\n');
        for (int i = 0; i < sizeof(test_table) / sizeof(test_number_t); i++) {
            if (event == test_table[i].num) {
                gptmr_config(event);
                continue;
            }
        }
        show_help();
    }
}

static void show_help(void)
{
    static const char help_info[] = "\n"
                                    "-----------------------------------------------------------------------------------\n"
                                    "*                                                                                 *\n"
                                    "*                   trigger dma transfer using gptmr event demo                   *\n"
                                    "*                                                                                 *\n";

    static const char help_info_ending[] = "*---------------------------------------------------------------------------------*\n";

    printf("%s", help_info);
    for (int i = 0; i < sizeof(test_table) / sizeof(test_number_t); i++) {
        printf("%s", test_table[i].num_str);
    }
    printf("%s", help_info_ending);
}

