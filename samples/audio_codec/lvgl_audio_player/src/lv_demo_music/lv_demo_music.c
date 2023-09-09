/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_music.h"

#if LV_USE_DEMO_MUSIC

#include "lv_demo_music_main.h"
#include "lv_demo_music_list.h"
#include "audio_codec_common.h"
#include "sd_fatfs_common.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#if LV_DEMO_MUSIC_AUTO_PLAY
static void auto_step_cb(lv_timer_t *timer);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *ctrl;
static lv_obj_t *list;

static char *title_list[20];
static char *artist_list[20];
static char *genre_list[20];
static uint32_t time_list[20];
static const char artist_default[] = "unknown artist";
static const char genre_default[] = "unknown genre";

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_disp_no_sd_card_info(void)
{
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "mount sd card error ");
    lv_obj_set_style_text_align(label, LV_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

int8_t lv_generate_music_info_table(void)
{
    FRESULT rsl;

    rsl = sd_search_music_name(".wav");
    if (rsl == FR_OK) {
        caculate_music_time(time_list);
        for (uint8_t i = 0; i < sd_get_search_file_cnt(); i++) {
            title_list[i] = sd_get_search_file_buff_ptr(i);
            artist_list[i] = (char *)artist_default;
            genre_list[i] = (char *)genre_default;
        }
    } else {
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "no \".wav\" music file");
        lv_obj_set_style_text_align(label, LV_ALIGN_CENTER, 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }

    return (int8_t)rsl;
}

void lv_demo_music(void)
{
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x343247), 0);

    list = _lv_demo_music_list_create(lv_scr_act());
    ctrl = _lv_demo_music_main_create(lv_scr_act());

#if LV_DEMO_MUSIC_AUTO_PLAY
    lv_timer_create(auto_step_cb, 1000, NULL);
#endif
}

char *_lv_demo_music_get_title(uint32_t track_id)
{
    if (track_id >= sd_get_search_file_cnt())
        return NULL;
    return title_list[track_id];
}

char *_lv_demo_music_get_artist(uint32_t track_id)
{
    if (track_id >= sd_get_search_file_cnt())
        return NULL;
    return artist_list[track_id];
}

char *_lv_demo_music_get_genre(uint32_t track_id)
{
    if (track_id >= sd_get_search_file_cnt())
        return NULL;
    return genre_list[track_id];
}

uint32_t _lv_demo_music_get_track_length(uint32_t track_id)
{
    if (track_id >= sd_get_search_file_cnt())
        return 0;
    return time_list[track_id];
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#if LV_DEMO_MUSIC_AUTO_PLAY
static void auto_step_cb(lv_timer_t *t)
{
    LV_UNUSED(t);
    static uint32_t state;

#if LV_DEMO_MUSIC_LARGE
    const lv_font_t *font_small = &lv_font_montserrat_22;
    const lv_font_t *font_large = &lv_font_montserrat_32;
#else
    const lv_font_t *font_small = &lv_font_montserrat_12;
    const lv_font_t *font_large = &lv_font_montserrat_16;
#endif

    switch (state) {
    case 5:
        _lv_demo_music_album_next(true);
        break;

    case 6:
        _lv_demo_music_album_next(true);
        break;
    case 7:
        _lv_demo_music_album_next(true);
        break;
    case 8:
        _lv_demo_music_play(0);
        break;
#if LV_DEMO_MUSIC_SQUARE || LV_DEMO_MUSIC_ROUND
    case 11:
        lv_obj_scroll_by(ctrl, 0, -LV_VER_RES, LV_ANIM_ON);
        break;
    case 13:
        lv_obj_scroll_by(ctrl, 0, -LV_VER_RES, LV_ANIM_ON);
        break;
#else
    case 12:
        lv_obj_scroll_by(ctrl, 0, -LV_VER_RES, LV_ANIM_ON);
        break;
#endif
    case 15:
        lv_obj_scroll_by(list, 0, -300, LV_ANIM_ON);
        break;
    case 16:
        lv_obj_scroll_by(list, 0, 300, LV_ANIM_ON);
        break;
    case 18:
        _lv_demo_music_play(1);
        break;
    case 19:
        lv_obj_scroll_by(ctrl, 0, LV_VER_RES, LV_ANIM_ON);
        break;
#if LV_DEMO_MUSIC_SQUARE || LV_DEMO_MUSIC_ROUND
    case 20:
        lv_obj_scroll_by(ctrl, 0, LV_VER_RES, LV_ANIM_ON);
        break;
#endif
    case 30:
        _lv_demo_music_play(2);
        break;
    case 40: {
        lv_obj_t *bg = lv_layer_top();
        lv_obj_set_style_bg_color(bg, lv_color_hex(0x6f8af6), 0);
        lv_obj_set_style_text_color(bg, lv_color_white(), 0);
        lv_obj_set_style_bg_opa(bg, LV_OPA_COVER, 0);
        lv_obj_fade_in(bg, 400, 0);
        lv_obj_t *dsc = lv_label_create(bg);
        lv_obj_set_style_text_font(dsc, font_small, 0);
        lv_label_set_text(dsc, "The average FPS is");
        lv_obj_align(dsc, LV_ALIGN_TOP_MID, 0, 90);

        lv_obj_t *num = lv_label_create(bg);
        lv_obj_set_style_text_font(num, font_large, 0);
#if LV_USE_PERF_MONITOR
        lv_label_set_text_fmt(num, "%d", lv_refr_get_fps_avg());
#endif
        lv_obj_align(num, LV_ALIGN_TOP_MID, 0, 120);

        lv_obj_t *attr = lv_label_create(bg);
        lv_obj_set_style_text_align(attr, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(attr, font_small, 0);
#if LV_DEMO_MUSIC_SQUARE || LV_DEMO_MUSIC_ROUND
        lv_label_set_text(attr, "Copyright 2020 LVGL Kft.\nwww.lvgl.io | lvgl@lvgl.io");
#else
        lv_label_set_text(attr, "Copyright 2020 LVGL Kft. | www.lvgl.io | lvgl@lvgl.io");
#endif
        lv_obj_align(attr, LV_ALIGN_BOTTOM_MID, 0, -10);
        break;
    }
    case 41:
        lv_scr_load(lv_obj_create(NULL));
        _lv_demo_music_pause();
        break;
    }
    state++;
}

#endif /*LV_DEMO_MUSIC_AUTO_PLAY*/

#endif /*LV_USE_DEMO_MUSIC*/
