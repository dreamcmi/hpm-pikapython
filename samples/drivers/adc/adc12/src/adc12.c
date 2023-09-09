/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "hpm_debug_console.h"
#include "hpm_adc12_drv.h"
#include "hpm_pwm_drv.h"
#include "hpm_trgm_drv.h"
#include "hpm_trgmmux_src.h"

#define APP_ADC12_CORE HPM_CORE0

#define APP_ADC12_SEQ_START_POS              (0U)
#define APP_ADC12_SEQ_IRQ_EVENT              adc12_event_seq_single_complete
#define APP_ADC12_SEQ_DMA_BUFF_LEN_IN_4BYTES (1024U)

#define APP_ADC12_PMT_PWM_REFCH_A            (8U)
#define APP_ADC12_PMT_PWM                    HPM_PWM0
#define APP_ADC12_PMT_TRGM                   HPM_TRGM0
#define APP_ADC12_PMT_TRGM_IN                HPM_TRGM0_INPUT_SRC_PWM0_CH8REF
#define APP_ADC12_PMT_TRGM_OUT               TRGM_TRGOCFG_ADCX_PTRGI0A
#define APP_ADC12_PMT_TRIG_CH                ADC12_CONFIG_TRG0A
#define APP_ADC12_PMT_IRQ_EVENT              adc12_event_trig_complete
#define APP_ADC12_PMT_DMA_BUFF_LEN_IN_4BYTES ADC_SOC_PMT_MAX_DMA_BUFF_LEN_IN_4BYTES

ATTR_PLACE_AT_NONCACHEABLE_WITH_ALIGNMENT(ADC_SOC_DMA_ADDR_ALIGNMENT) uint32_t seq_buff[APP_ADC12_SEQ_DMA_BUFF_LEN_IN_4BYTES];
ATTR_PLACE_AT_NONCACHEABLE_WITH_ALIGNMENT(ADC_SOC_DMA_ADDR_ALIGNMENT) uint32_t pmt_buff[APP_ADC12_PMT_DMA_BUFF_LEN_IN_4BYTES];

uint8_t seq_adc_channel[] = {BOARD_APP_ADC12_CH_1};
uint8_t trig_adc_channel[] = {BOARD_APP_ADC12_CH_1};
__IO uint8_t seq_full_complete_flag;
__IO uint8_t trig_complete_flag;

static uint8_t get_adc_conv_mode(void)
{
    uint8_t ch;

    while (1) {
        printf("1. Oneshot    mode\n");
        printf("2. Period     mode\n");
        printf("3. Sequence   mode\n");
        printf("4. Preemption mode\n");

        printf("Please enter one of ADC conversion modes above (e.g. 1 or 2 ...): ");
        printf("%c\n", ch = getchar());
        ch -= '0' + 1;
        if (ch > adc12_conv_mode_preemption) {
            printf("The ADC mode is not supported!\n");
        } else {
            return ch;
        }
    }
}

void isr_adc12(void)
{
    uint32_t status;

    status = adc12_get_status_flags(BOARD_APP_ADC12_BASE);

    if (ADC12_INT_STS_SEQ_CMPT_GET(status)) {
        /* Clear seq_complete status */
        adc12_clear_status_flags(BOARD_APP_ADC12_BASE, adc12_event_seq_full_complete);
        /* Set flag to read memory data */
        seq_full_complete_flag = 1;
    }

    if (ADC12_INT_STS_TRIG_CMPT_GET(status)) {
        /* Clear trig_cmpt status */
        adc12_clear_status_flags(BOARD_APP_ADC12_BASE, adc12_event_trig_complete);
        /* Set flag to read memory data */
        trig_complete_flag = 1;
    }
}
SDK_DECLARE_EXT_ISR_M(BOARD_APP_ADC12_IRQn, isr_adc12)

hpm_stat_t process_seq_data(uint32_t *buff, uint32_t start_pos, uint32_t len)
{
    adc12_seq_dma_data_t *dma_data = (adc12_seq_dma_data_t *)buff;

    if (ADC12_IS_SEQ_DMA_BUFF_LEN_INVLAID(len)) {
        return status_invalid_argument;
    }

    for (int i = start_pos; i < start_pos + len; i++) {
        printf("Sequence Mode - %s - ", BOARD_APP_ADC12_NAME);
        printf("Cycle Bit: %02d - ",   dma_data[i].cycle_bit);
        printf("Sequence Number:%02d - ", dma_data[i].seq_num);
        printf("ADC Channel: %02d - ",  dma_data[i].adc_ch);
        printf("Result: 0x%04x\n", dma_data[i].result);
    }

    return status_success;
}

hpm_stat_t process_pmt_data(uint32_t *buff, int32_t start_pos, uint32_t len)
{
    adc12_pmt_dma_data_t *dma_data = (adc12_pmt_dma_data_t *)buff;

    if (ADC12_IS_PMT_DMA_BUFF_LEN_INVLAID(len)) {
        return status_invalid_argument;
    }

    for (int i = start_pos; i < start_pos + len; i++) {
        if (dma_data[i].cycle_bit) {
            printf("Preemption Mode - %s - ", BOARD_APP_ADC12_NAME);
            printf("Trigger Channel: %02d - ", dma_data[i].trig_ch);
            printf("Cycle Bit: %02d - ", dma_data[i].cycle_bit);
            printf("Sequence Number: %02d - ", dma_data[i].seq_num);
            printf("ADC Channel: %02d - ", dma_data[i].adc_ch);
            printf("Result: 0x%04x\n", dma_data[i].result);
            dma_data[i].cycle_bit = 0;
        } else {
            printf("invalid data\n");
        }
    }

    return status_success;
}

void init_trigger_source(PWM_Type *ptr)
{
    pwm_cmp_config_t pwm_cmp_cfg;
    pwm_output_channel_t pwm_output_ch_cfg;

    /* TODO: Set PWM Clock Source and divider */

    /* 33.33KHz reload at 200MHz */
    pwm_set_reload(ptr, 0, 5999);

    /* Set a comparator */
    memset(&pwm_cmp_cfg, 0x00, sizeof(pwm_cmp_config_t));
    pwm_cmp_cfg.enable_ex_cmp  = false;
    pwm_cmp_cfg.mode           = pwm_cmp_mode_output_compare;
    pwm_cmp_cfg.update_trigger = pwm_shadow_register_update_on_shlk;

    /* Select comp8 and trigger at the middle of a pwm cycle */
    pwm_cmp_cfg.cmp = 2999;
    pwm_config_cmp(ptr, APP_ADC12_PMT_PWM_REFCH_A, &pwm_cmp_cfg);

    /* Issue a shadow lock */
    pwm_issue_shadow_register_lock_event(APP_ADC12_PMT_PWM);

    /* Set comparator channel to generate a trigger signal */
    pwm_output_ch_cfg.cmp_start_index = APP_ADC12_PMT_PWM_REFCH_A;   /* start channel */
    pwm_output_ch_cfg.cmp_end_index   = APP_ADC12_PMT_PWM_REFCH_A;   /* end channel */
    pwm_output_ch_cfg.invert_output   = false;
    pwm_config_output_channel(ptr, APP_ADC12_PMT_PWM_REFCH_A, &pwm_output_ch_cfg);

	/* Start the comparator counter */
    pwm_start_counter(ptr);
}

void init_trigger_mux(TRGM_Type * ptr)
{
    trgm_output_t trgm_output_cfg;

    trgm_output_cfg.invert = false;
    trgm_output_cfg.type = trgm_output_same_as_input;

    trgm_output_cfg.input  = APP_ADC12_PMT_TRGM_IN;
    trgm_output_config(ptr, APP_ADC12_PMT_TRGM_OUT, &trgm_output_cfg);
}

void init_trigger_cfg(ADC12_Type *ptr, uint8_t trig_ch, bool inten)
{
    adc12_pmt_config_t pmt_cfg;

    pmt_cfg.trig_len = sizeof(trig_adc_channel);
    pmt_cfg.trig_ch = trig_ch;

    for (int i = 0; i < pmt_cfg.trig_len; i++) {
        pmt_cfg.adc_ch[i] = trig_adc_channel[i];
        pmt_cfg.inten[i] = false;
    }

    pmt_cfg.inten[pmt_cfg.trig_len - 1] = inten;

    adc12_set_pmt_config(ptr, &pmt_cfg);
}

void init_common_config(adc12_conversion_mode_t conv_mode)
{
    adc12_config_t cfg;

    /* initialize an ADC instance */
    adc12_get_default_config(&cfg);

    cfg.res            = adc12_res_12_bits;
    cfg.conv_mode      = conv_mode;
    cfg.adc_clk_div    = adc12_clock_divider_3;
    cfg.sel_sync_ahb   = false;
    if (cfg.conv_mode == adc12_conv_mode_sequence ||
        cfg.conv_mode == adc12_conv_mode_preemption) {
        cfg.adc_ahb_en = true;
    }

    adc12_init(BOARD_APP_ADC12_BASE, &cfg);

    /* enable irq */
    intc_m_enable_irq_with_priority(BOARD_APP_ADC12_IRQn, 1);
}

void init_oneshot_config(void)
{
    adc12_channel_config_t ch_cfg;

    /* get a default channel config */
    adc12_get_channel_default_config(&ch_cfg);

    /* initialize an ADC channel */
    ch_cfg.ch           = BOARD_APP_ADC12_CH_1;
    ch_cfg.diff_sel     = adc12_sample_signal_single_ended;
    ch_cfg.sample_cycle = 20;

    adc12_init_channel(BOARD_APP_ADC12_BASE, &ch_cfg);
}

void oneshot_handler(void)
{
    uint16_t result;

    if (adc12_get_oneshot_result(BOARD_APP_ADC12_BASE, BOARD_APP_ADC12_CH_1, &result) == status_success) {
        printf("Oneshot Mode - %s [channel %d] - Result: 0x%04x\n", BOARD_APP_ADC12_NAME, BOARD_APP_ADC12_CH_1, result);
    }
}

void init_period_config(void)
{
    adc12_channel_config_t ch_cfg;
    adc12_prd_config_t prd_cfg;

    /* get a default channel config */
    adc12_get_channel_default_config(&ch_cfg);

    /* initialize an ADC channel */
    ch_cfg.ch           = BOARD_APP_ADC12_CH_1;
    ch_cfg.diff_sel     = adc12_sample_signal_single_ended;
    ch_cfg.sample_cycle = 20;

    adc12_init_channel(BOARD_APP_ADC12_BASE, &ch_cfg);

    prd_cfg.ch           = BOARD_APP_ADC12_CH_1;
    prd_cfg.prescale     = 22;    /* Set divider: 2^22 clocks */
    prd_cfg.period_count = 5;     /* 104.86ms when AHB clock at 200MHz is ADC clock source */

    adc12_set_prd_config(BOARD_APP_ADC12_BASE, &prd_cfg);
}

void period_handler(void)
{
    uint16_t result;

    adc12_get_prd_result(BOARD_APP_ADC12_BASE, BOARD_APP_ADC12_CH_1, &result);
    printf("Period Mode - %s [channel %d] - Result: 0x%04x\n", BOARD_APP_ADC12_NAME, BOARD_APP_ADC12_CH_1, result);
}

void init_sequence_config(void)
{
    adc12_seq_config_t seq_cfg;
    adc12_dma_config_t dma_cfg;
    adc12_channel_config_t ch_cfg;

    /* get a default channel config */
    adc12_get_channel_default_config(&ch_cfg);

    /* initialize an ADC channel */
    ch_cfg.diff_sel     = adc12_sample_signal_single_ended;
    ch_cfg.sample_cycle = 20;

    for (int i = 0; i < sizeof(seq_adc_channel); i++) {
        ch_cfg.ch           = seq_adc_channel[i];
        adc12_init_channel(BOARD_APP_ADC12_BASE, &ch_cfg);
    }

    /* Set a sequence config */
    seq_cfg.seq_len    = sizeof(seq_adc_channel);
    seq_cfg.restart_en = false;
    seq_cfg.cont_en    = true;
    seq_cfg.sw_trig_en = true;
    seq_cfg.hw_trig_en = true;

    for (int i = APP_ADC12_SEQ_START_POS; i < seq_cfg.seq_len; i++) {
        seq_cfg.queue[i].seq_int_en = false;
        seq_cfg.queue[i].ch = seq_adc_channel[i];
    }

     /* Enable the single complete interrupt for the last conversion */
    seq_cfg.queue[seq_cfg.seq_len - 1].seq_int_en = true;

    /* Initialize a sequence */
    adc12_set_seq_config(BOARD_APP_ADC12_BASE, &seq_cfg);

    /* Set a DMA config */
    dma_cfg.start_addr         = (uint32_t *)core_local_mem_to_sys_address(APP_ADC12_CORE, (uint32_t)seq_buff);
    dma_cfg.buff_len_in_4bytes = sizeof(seq_adc_channel);
    dma_cfg.stop_en            = false;
    dma_cfg.stop_pos           = 0;

    /* Initialize DMA for the sequence mode */
    adc12_init_seq_dma(BOARD_APP_ADC12_BASE, &dma_cfg);

    /* Enable sequence complete interrupt */
    adc12_enable_interrupts(BOARD_APP_ADC12_BASE, APP_ADC12_SEQ_IRQ_EVENT);
}

void sequence_handler(void)
{
    /* SW trigger */
    adc12_trigger_seq_by_sw(BOARD_APP_ADC12_BASE);

    while (seq_full_complete_flag == 0) {

    }

    /* Process data */
    process_seq_data(seq_buff, APP_ADC12_SEQ_START_POS, sizeof(seq_adc_channel));

    /* Clear the flag */
    seq_full_complete_flag = 0;
}

void init_preemption_config(void)
{
    adc12_channel_config_t ch_cfg;

    /* get a default channel config */
    adc12_get_channel_default_config(&ch_cfg);

    /* initialize an ADC channel */
    ch_cfg.diff_sel     = adc12_sample_signal_single_ended;
    ch_cfg.sample_cycle = 20;

    for (int i = 0; i < sizeof(trig_adc_channel); i++) {
        ch_cfg.ch = trig_adc_channel[i];
        adc12_init_channel(BOARD_APP_ADC12_BASE, &ch_cfg);
    }

    /* Trigger source initialization */
    init_trigger_source(APP_ADC12_PMT_PWM);

    /* Trigger mux initialization */
    init_trigger_mux(APP_ADC12_PMT_TRGM);

    /* Trigger config initialization */
    init_trigger_cfg(BOARD_APP_ADC12_BASE, APP_ADC12_PMT_TRIG_CH, true);

    /* Set DMA start address for preemption mode */
    adc12_init_pmt_dma(BOARD_APP_ADC12_BASE, core_local_mem_to_sys_address(APP_ADC12_CORE, (uint32_t)pmt_buff));

    /* Enable trigger complete interrupt */
    adc12_enable_interrupts(BOARD_APP_ADC12_BASE, APP_ADC12_PMT_IRQ_EVENT);
}

void preemption_handler(void)
{
    /* Wait for a complete of conversion */
    while (trig_complete_flag == 0) {

    }

    /* Process data */
    process_pmt_data(pmt_buff, APP_ADC12_PMT_TRIG_CH * sizeof(adc12_pmt_dma_data_t), sizeof(trig_adc_channel));

    /* Clear the flag */
    trig_complete_flag = 0;
}

int main(void)
{
    uint8_t conv_mode;

    /* Bsp initialization */
    board_init();

    /* ADC pin initialization */
    board_init_adc12_pins();

    /* ADC clock initialization */
    board_init_adc12_clock(BOARD_APP_ADC12_BASE);

    printf("This is an ADC12 demo:\n");

    /* Get a conversion mode from a console window */
    conv_mode = get_adc_conv_mode();

    /* ADC12 common initialization */
    init_common_config(conv_mode);

    /* ADC12 read patter and DMA initialization */
    switch (conv_mode) {
        case adc12_conv_mode_oneshot:
            init_oneshot_config();
            break;

        case adc12_conv_mode_period:
            init_period_config();
            break;

        case adc12_conv_mode_sequence:
            init_sequence_config();
            break;

        case adc12_conv_mode_preemption:
            init_preemption_config();
            break;

        default:
            break;
    }

    /* Main loop */
    while (1) {
        board_delay_ms(1000);

        if (conv_mode == adc12_conv_mode_oneshot) {
            oneshot_handler();
        } else if (conv_mode == adc12_conv_mode_period) {
            period_handler();
        } else if (conv_mode == adc12_conv_mode_sequence) {
            sequence_handler();
        } else if (conv_mode == adc12_conv_mode_preemption) {
            preemption_handler();
        } else {
            printf("Conversion mode is not supported!\n");
        }
    }
}
