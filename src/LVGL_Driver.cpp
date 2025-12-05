/*****************************************************************************
  | File        :   LVGL_Driver.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include "LVGL_Driver.h"

static uint8_t buf1[LVGL_BUF_LEN * sizeof(lv_color_t)];
static uint8_t buf2[LVGL_BUF_LEN * sizeof(lv_color_t)];

/* Serial debugging */
void Lvgl_print(const char * buf)
{
    // Serial.printf(buf);
    // Serial.flush();
}

/*  Display flushing 
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
void Lvgl_Display_LCD(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  LCD_addWindow(area->x1, area->y1, area->x2, area->y2, (uint16_t *)px_map);
  lv_display_flush_ready(disp);
}

/*Read the touchpad*/
void Lvgl_Touchpad_Read(lv_indev_t * indev, lv_indev_data_t * data)
{
  uint16_t touchpad_x[5] = {0};
  uint16_t touchpad_y[5] = {0};
  uint16_t strength[5]   = {0};
  uint8_t touchpad_cnt = 0;
  Touch_Read_Data();
  uint8_t touchpad_pressed = Touch_Get_XY(touchpad_x, touchpad_y, strength, &touchpad_cnt, CST328_LCD_TOUCH_MAX_POINTS);
  if (touchpad_pressed && touchpad_cnt > 0) {
    data->point.x = touchpad_x[0];
    data->point.y = touchpad_y[0];
    data->state = LV_INDEV_STATE_PRESSED;
    // printf("LVGL  : X=%u Y=%u num=%d\r\n", touchpad_x[0], touchpad_y[0],touchpad_cnt);
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

void Lvgl_Init(void)
{
  lv_init();

  /*Initialize the display*/
  lv_display_t * disp = lv_display_create(LVGL_WIDTH, LVGL_HEIGHT);
  lv_display_set_flush_cb(disp, Lvgl_Display_LCD);
  lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  /*Initialize the input device driver*/
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, Lvgl_Touchpad_Read);

  /* Create simple label */
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello Arduino and LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);

}

void Lvgl_Loop(void)
{
  lv_timer_handler(); /* let the GUI do its work */
}
