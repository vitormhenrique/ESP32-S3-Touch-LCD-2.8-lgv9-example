#pragma once

#include <lvgl.h>
#include "lv_conf.h"
#include <demos/lv_demos.h>
#include <esp_heap_caps.h>
#include "Display_ST7789.h"
#include "Touch_CST328.h"

#define LVGL_WIDTH     LCD_WIDTH
#define LVGL_HEIGHT    LCD_HEIGHT
#define LVGL_BUF_LEN  (LVGL_WIDTH * LVGL_HEIGHT / 10)

#define EXAMPLE_LVGL_TICK_PERIOD_MS  2


void Lvgl_print(const char * buf);
void Lvgl_Display_LCD(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map); // Displays LVGL content on the LCD
void Lvgl_Touchpad_Read(lv_indev_t * indev, lv_indev_data_t * data);                // Read the touchpad
void example_increase_lvgl_tick(void *arg);

void Lvgl_Init(void);
void Lvgl_Loop(void);
