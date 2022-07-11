
#pragma once

#include "lvgl/lvgl.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_AAAAAY
#define LV_ATTRIBUTE_IMG_AAAAAY
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_AAAAAY uint8_t aaaaay_map[] = {
    };


const lv_img_dsc_t aaaaay = {
    {
        LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
        0,
        0,
        240,
        240
    },
    115200,
    aaaaay_map
};
