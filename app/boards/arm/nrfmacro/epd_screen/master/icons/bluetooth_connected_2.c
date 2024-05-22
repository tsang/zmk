/*
*
* Copyright (c) 2021 Darryl deHaan
* SPDX-License-Identifier: MIT
*
*/

#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif
#ifndef LV_ATTRIBUTE_IMG_BLUETOOTH_CONNECTED_2
#define LV_ATTRIBUTE_IMG_BLUETOOTH_CONNECTED_2
#endif
const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_BLUETOOTH_CONNECTED_2 uint8_t bluetooth_connected_2_map[] = {
  0xff, 0xff, 0xff, 0xff, 	/*Color of index 0*/
  0x00, 0x00, 0x00, 0xff, 	/*Color of index 1*/

  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x7e, 0x00, 0x00, 0x03, 0xfc, 0x00, 
  0x00, 0x7f, 0x00, 0x00, 0x0f, 0xff, 0x00, 
  0x30, 0x7f, 0x80, 0x00, 0x1f, 0xff, 0xc0, 
  0x78, 0x73, 0xc0, 0x00, 0x7f, 0xff, 0xe0, 
  0x7c, 0x71, 0xe0, 0x00, 0x7f, 0xff, 0xf0, 
  0x3e, 0x73, 0xe0, 0x00, 0xff, 0x0f, 0xf0, 
  0x1f, 0x77, 0xc0, 0x01, 0xfe, 0x07, 0xf8, 
  0x0f, 0xff, 0x80, 0x01, 0xfc, 0x63, 0xf8, 
  0x87, 0xff, 0x08, 0x01, 0xff, 0xf3, 0xfc, 
  0xc3, 0xfe, 0x18, 0x03, 0xff, 0xe3, 0xfc, 
  0xe1, 0xfc, 0x38, 0x03, 0xff, 0xe7, 0xfc, 
  0xf0, 0xf8, 0x78, 0x03, 0xff, 0xc7, 0xfc, 
  0xe1, 0xfc, 0x38, 0x03, 0xff, 0x8f, 0xfc, 
  0xc3, 0xfe, 0x18, 0x03, 0xff, 0x1f, 0xfc, 
  0x87, 0xff, 0x08, 0x01, 0xfe, 0x3f, 0xfc, 
  0x0f, 0xff, 0x80, 0x01, 0xfc, 0x03, 0xf8, 
  0x1f, 0x77, 0xc0, 0x01, 0xfc, 0x03, 0xf8, 
  0x3e, 0x73, 0xe0, 0x00, 0xff, 0xff, 0xf0, 
  0x7c, 0x71, 0xe0, 0x00, 0x7f, 0xff, 0xf0, 
  0x78, 0x73, 0xc0, 0x00, 0x7f, 0xff, 0xe0, 
  0x30, 0x7f, 0x80, 0x00, 0x1f, 0xff, 0xc0, 
  0x00, 0x7f, 0x00, 0x00, 0x0f, 0xff, 0x00, 
  0x00, 0x7e, 0x00, 0x00, 0x03, 0xfc, 0x00, 
  0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t bluetooth_connected_2 = {
  .header.always_zero = 0,
  .header.w = 54,
  .header.h = 35,
  .data_size = 254,
  .header.cf = LV_IMG_CF_INDEXED_1BIT,
  .data = bluetooth_connected_2_map,
};
