/*
 * Copyright (c) 2022-2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _AUDIO_V2_SPEAKER_H
#define _AUDIO_V2_SPEAKER_H


/*
 * Function Declaration
 */
void cherryusb_audio_v2_init(void);
void speaker_init_i2s_dao_codec(void);
void mic_init_i2s_pdm(void);
void i2s_enable_dma_irq_with_priority(int32_t priority);
void cherryusb_audio_v2_main_task(void);


#endif