#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lcdapp.h"
#include "lvgl_lcd.h"

const static char *TAG = "LVGL TEST";

void app_main(void)
{
    lvgl_lcd_init();
    lcdapp_init();
    ESP_LOGI(TAG, "main");
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
