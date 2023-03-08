#ifndef __ST7796S_H__
#define __ST7796S_H__

#include "tftglobal.h"
/*
 * Display Output Ctrl Adjust REG
 * if you want to change Source timing Control(us)  or Gate timing Control (Tclk)
 * You must write
 * stparamete1: 0x40
 * stparamete2: 0x8A
 * stparamete3: 0x00
 * stparamete4: 0x00
 * stparamete5:      
 *  BIT[0:3]:
        BIT [3:0]   Source timing Control(us)
        00h         9
        01h         10.5
        02h         12
        03h         13.5
        :
        :
        0Eh         30
        0Fh         31.5 
    BIT[4:7]:0x25
 *  stparamete6:
    BIT[0:5]:
        
 */
#define DOCA                    0xE8

/*
 * COMMAND SET CONTROL REG
 * ENABLE COMMAND:
 *  You must write 0xC3 first and then 0x96
 * DISABLE COMMAND:
 *  You must write 0x3C first and then 0x69
 */
#define COMMAND_SET_CONTROL     0xF0

/*
 * SPI READ CONTROL REG
 * BIT4:
 *  1:read enable
 *  0:read disable
 * BIT0~3:
 *  read parameter number
 */
#define SPI_READ_CONTROL        0xFB


void st7796s_init(struct tftlcd_config *config);


#endif /* __ST7796S_H__ */
