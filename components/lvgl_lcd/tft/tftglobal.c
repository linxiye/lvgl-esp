#include "tftglobal.h"
#include "string.h"
#include "stdio.h"
#include "esp_heap_caps.h"
static struct tftlcd_config *dev;
static void send_cmd(uint8_t cmd)
{
	dev->dc_value(write_cmd);
	dev->spi_write_data(&cmd, 1);
}

static void send_data(void *data, uint32_t length)
{
	dev->dc_value(write_data);
	dev->spi_write_data((uint8_t *)data, length);
}

void screen_frambuffer_write(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint32_t len;
	uint8_t data[4];

	/*Column addresses*/
	send_cmd(0x2A);
	data[0] = (x1 >> 8) & 0xFF;
	data[1] = x1 & 0xFF;
	data[2] = (x2 >> 8) & 0xFF;
	data[3] = x2 & 0xFF;
	send_data(data, 4);

	/*Page addresses*/
	send_cmd(0x2B);
	data[0] = (y1 >> 8) & 0xFF;
	data[1] = y1 & 0xFF;
	data[2] = (y2 >> 8) & 0xFF;
	data[3] = y2 & 0xFF;
	send_data(data, 4);	

	send_cmd(0x2C);
	len = (x2 - x1) * (y2 - y1) * 2;
	send_data(&dev->Framebuffer[y1 * dev->lcd_x], len);
}


void screen_write(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,void *colour)
{
	uint16_t *value = (uint16_t *)colour;
	for(int i = 0;i<(y2 - y1);i++)
		for(int j = 0;j<(x2 - x1);j++)
			dev->Framebuffer[(y1 + i) * dev->lcd_x + x1 + j] = value[i];
}

void dma_callback(void)
{
	screen_frambuffer_write(0,0,240,40);
	screen_frambuffer_write(0,40,240,80);
	screen_frambuffer_write(0,80,240,120);
	screen_frambuffer_write(0,120,240,160);
	screen_frambuffer_write(0,160,240,200);
	screen_frambuffer_write(0,200,240,240);
}

void tftlcd_register(struct tftlcd_config *config,lcd_init_cmd_t *init_cmds)
{
	uint16_t i = 0;
	dev = config;

	if(dev->res_value != NULL){
		dev->res_value(RESET_ON);
		dev->delayms(100);
		dev->res_value(RESET_OFF);
		dev->delayms(100);
	}
	else {
		dev->delayms(200);
	}

    while(init_cmds[i].databytes != 0xFF){
		send_cmd(init_cmds[i].cmd);
		send_data(init_cmds[i].data, init_cmds[i].databytes);	
		i++;
	}

	if(dev->blk_value != NULL){
		dev->blk_value(BLK_ON);
	}
	dev->initialize = 1;
}