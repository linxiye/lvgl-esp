#include "lcdapp.h"
#include "tftglobal.h"
#include <driver/spi_master.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include <esp_timer.h>
#include <string.h>
#include <lvgl.h>
#include <st7789.h>
#include <lvgl_lcd.h>
#include <esp_log.h>

const static char *TAG = "lcdapp";

#define CLK 	12
#define MOSI	11
#define RES	    10
#define DC	    14
#define MISO	13
#define BLK	    21
#define CS      9

spi_device_handle_t SPIHandle;
spi_transaction_t t;

void lcdapp_delayms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void lcdapp_dc_value(dc_t value)
{
    gpio_set_level(DC,value);
}

void lcdapp_blk_value(blk_t value)
{
    gpio_set_level(BLK,value);
}

void lcdapp_res_value(res_t value)
{
    gpio_set_level(RES,value);
}

void lcdapp_spi_write_data(uint8_t *data,uint32_t len)
{
    esp_err_t ret;
    
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length = len * 8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=&data[0];           //Data
    t.flags = 0;
    // ret = spi_device_queue_trans(SPIHandle,&t,portMAX_DELAY);
    ret=spi_device_polling_transmit(SPIHandle, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.

}


static struct tftlcd_config config = {
    .lcd_x = 240,
    .lcd_y = 240,
    .initialize = 0,
    .delayms = lcdapp_delayms,
    .dc_value = lcdapp_dc_value,
    .blk_value = lcdapp_blk_value,
    .res_value = lcdapp_res_value,
    .spi_write_data = lcdapp_spi_write_data,
};


static void lv_tick_task(void *arg){
    (void) arg;
        lv_tick_inc(10);
}


void lcdapp_task(void *arg)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_task_handler();
        if(config.initialize == 1){
            dma_callback();
        }
    }
}

void lcdapp_init(void)
{
    esp_err_t ret;
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //bit mask of the pins, use GPIO here
    io_conf.pin_bit_mask = (1ULL << RES) | (1ULL << DC) | (1ULL << BLK) |  (1ULL << CS);
    //set as input mode
    io_conf.mode = GPIO_MODE_OUTPUT;

    gpio_config(&io_conf);

    spi_bus_config_t buscfg = {
		.mosi_io_num = MOSI,
		.miso_io_num = MISO,
		.sclk_io_num = CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = (config.lcd_x * config.lcd_y) * 4 + 8,
		.flags = 0
	};

    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));
	devcfg.clock_speed_hz = SPI_MASTER_FREQ_80M;
	devcfg.queue_size = 10;
	devcfg.mode = 3;
	devcfg.flags = SPI_DEVICE_NO_DUMMY;
    devcfg.spics_io_num = CS;
	devcfg.input_delay_ns = 6;			//从机最大数据有效时间    
	ret = spi_bus_add_device(SPI2_HOST, &devcfg, &SPIHandle);
	assert(ret==ESP_OK);

    config.Framebuffer = heap_caps_malloc(sizeof(uint16_t) * config.lcd_x * config.lcd_y,MALLOC_CAP_DMA);
    st7789_init(&config);
    vTaskDelay(pdMS_TO_TICKS(100));

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 10000)); 
    xTaskCreatePinnedToCore(lcdapp_task, "lcdapp_init", 4096 * 3, NULL, 4, NULL, 1);   
}