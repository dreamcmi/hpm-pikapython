/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _AUDIO_V1_MIC_SPEAKER_MIDI_H
#define _AUDIO_V1_MIC_SPEAKER_MIDI_H

/*
 * Function Declaration
 */
void audio_init(void);
void audio_task(void);
void midi_task(void);
void i2s_enable_dma_irq_with_priority(int32_t priority);
void mic_init_i2s_pdm(void);
void speaker_init_i2s_dao(void);


#endif