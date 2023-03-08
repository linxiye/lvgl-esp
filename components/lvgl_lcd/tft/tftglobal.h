#ifndef __TFTGLOBAL_H__
#define __TFTGLOBAL_H__

#include "stdint.h"

/*The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct. */
typedef struct
{
	uint8_t cmd;
	uint8_t data[16];
	uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

typedef enum __st7796s_mode_t {
    SPI_MODE = 0x00,
    P8080_MODE_16Bit = 0x01,
}st7796s_mode_t;

typedef enum __horizontal_t{
    DISP_ROT_0 = 0x2,
    DISP_ROT_90 = 0x4,
    DISP_ROT_180 = 0x8,
    DISP_ROT_270 = 0xE
}horizontal_t;

typedef enum __rgb_bgr_order_t{
    ORDER_RGB = 0x00,
    ORDER_BGR = 0x01,
}rgb_bgr_order_t;

typedef enum __dc_t {
    write_cmd = 0x00,
    write_data = 0x01,
}dc_t;

typedef enum __blk_t{
    BLK_OFF = 0x00,
    BLK_ON = 0x01,
}blk_t;

typedef enum __res_t{
    RESET_ON = 0x00,
    RESET_OFF = 0x01,
}res_t;


struct tftlcd_config{
    uint16_t lcd_x;
    uint16_t lcd_y;
    uint16_t *Framebuffer;
    uint8_t initialize;
    void (*delayms)(uint32_t delayms);
    void (*dc_value)(dc_t value);
    void (*blk_value)(blk_t value);
    void (*res_value)(res_t value);
    void (*spi_write_data)(uint8_t *data,uint32_t len);
    void (*spi_read_data)(uint8_t *data,uint32_t len);
};


void dma_callback(void);
void tftlcd_register(struct tftlcd_config *config,lcd_init_cmd_t *init_cmds);
void screen_write(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,void *colour);

#endif