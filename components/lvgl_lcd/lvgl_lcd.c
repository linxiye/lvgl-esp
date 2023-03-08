#include "stdio.h"
#include "lvgl_lcd.h"
#include "lvgl.h"
#include "esp_heap_caps.h"
#include "demos/lv_demos.h"
#include "lvgl.h"
#include "tftglobal.h"
#include "esp_log.h"

#define lvgl_buff   (240 * 240)/10

static lv_color_t *buf1,*buf2;

static void lvgl_lcd_disp_draw(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
    // ESP_LOGI("Draw","%d %d %d %d %d",area->x1, area->y1, area->x2, area->y2,color_p[0].full);
    // screen_write(area->x1, area->y1, area->x2, area->y2, (uint16_t*)color_p);
    screen_write(area->x1, area->y1, area->x2, area->y2, (uint16_t*)color_p);
    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

void lvgl_lcd_init(void)
{
    lv_init();

    buf1 = heap_caps_malloc(sizeof(lv_color_t) * lvgl_buff,MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    buf2 = heap_caps_malloc(sizeof(lv_color_t) * lvgl_buff,MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, lvgl_buff);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;

    disp_drv.flush_cb = lvgl_lcd_disp_draw;

    disp_drv.draw_buf = &disp_buf;

    lv_disp_drv_register(&disp_drv);

    lv_demo_benchmark(LV_DEMO_BENCHMARK_MODE_RENDER_AND_DRIVER);
}