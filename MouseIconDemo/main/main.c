#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "ssd1306.h"

#include "usb_hid.h"

QueueHandle_t app_event_hid = NULL;

void usb_hid_task(void *pvParameters);

/*
 You have to set this config value with menuconfig
 CONFIG_INTERFACE

 for i2c
 CONFIG_MODEL
 CONFIG_SDA_GPIO
 CONFIG_SCL_GPIO
 CONFIG_RESET_GPIO

 for SPI
 CONFIG_CS_GPIO
 CONFIG_DC_GPIO
 CONFIG_RESET_GPIO
*/

#define TAG "SSD1306"

#define IMAGES 8
// https://thenounproject.com/icon/mouse-6617048/
uint8_t mouse[IMAGES][512] = {
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x82, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0x7c, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0xfe, 0x7f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xfe, 0x7f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x7f, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xcf, 0xfe, 0x7f, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfc, 0x3f, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xf9, 0x9f, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xfb, 0xdf, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xf3, 0xcf, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf3, 0xcf, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xf3, 0xcf, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf3, 0xcf, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfb, 0xdf, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf9, 0x9f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xf8, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x1f, 0xfe, 0x7f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x80, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7c, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x00, 0x7e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x7f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x7f, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x00, 0x7f, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x01, 0x9f, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xdf, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x03, 0xdf, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xcf, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x03, 0xcf, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xcf, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x03, 0xdf, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x9f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x7f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x82, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0x00, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xfe, 0x00, 0x1f, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x00, 0x07, 0xff, 0xff, 
0xff, 0xff, 0xcf, 0xfe, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfc, 0x00, 0x03, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xf9, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xfb, 0xc0, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xf3, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf3, 0xc0, 0x01, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xf3, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf3, 0xc0, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfb, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf9, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x1f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x82, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0x7c, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0xfe, 0x7e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xfe, 0x7f, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xfe, 0x7f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x7f, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xcf, 0xfe, 0x7f, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfc, 0x3f, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xf8, 0x1f, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xf8, 0x1f, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xf0, 0x0f, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf0, 0x0f, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xf0, 0x0f, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf0, 0x0f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xf0, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf8, 0x1f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xf8, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x1f, 0xfe, 0x7f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0x80, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0x80, 0x1f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x01, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0x80, 0x07, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x01, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xc0, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x03, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xc0, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x03, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03, 0xc0, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x03, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x00, 0x41, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x7c, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x00, 0x7e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x00, 0x7f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x7f, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x00, 0x7f, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x01, 0x9f, 0xfb, 0xff, 0xff, 0xff, 0xff, 0x80, 0x02, 0x5f, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x02, 0x4f, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x80, 0x02, 0x4f, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x02, 0x4f, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x80, 0x02, 0x4f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x02, 0x5f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x9f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x1f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x7f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x82, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0x00, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0xfe, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xfe, 0x00, 0x1f, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x00, 0x07, 0xff, 0xff, 
0xff, 0xff, 0xcf, 0xfe, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xfc, 0x00, 0x03, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xf9, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xfa, 0x40, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xf2, 0x40, 0x01, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf2, 0x40, 0x01, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xf2, 0x40, 0x01, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf2, 0x40, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfa, 0x40, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xf9, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x1f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
},
{
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x01, 0x80, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x01, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x01, 0x80, 0x1f, 0xff, 0xff, 
0xff, 0xff, 0xf0, 0x01, 0x80, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x01, 0x80, 0x07, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x03, 0xff, 0xff, 
0xff, 0xff, 0xc0, 0x01, 0x80, 0x03, 0xff, 0xff, 0xff, 0xff, 0x80, 0x02, 0x40, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x02, 0x40, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x02, 0x40, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x80, 0x02, 0x40, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x02, 0x40, 0x01, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x02, 0x40, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x80, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfd, 0xff, 0xff, 
0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xf9, 0xff, 0xff, 
0xff, 0xff, 0xdf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xf3, 0xff, 0xff, 
0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 
0xff, 0xff, 0xf3, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x9f, 0xff, 0xff, 
0xff, 0xff, 0xfc, 0x7f, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xf8, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x81, 0x81, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
}
};

void app_main(void)
{
	SSD1306_t dev;

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is i2c");
	ESP_LOGI(TAG, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(TAG, "INTERFACE is SPI");
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(TAG, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
	ESP_LOGI(TAG, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(TAG, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(TAG, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64
#if CONFIG_SSD1306_128x32
	ESP_LOGE(TAG, "Panel is 128x32. This demo cannot be run.");
	while(1) { vTaskDelay(1); }
#endif // CONFIG_SSD1306_128x32

	// Allocate memory
	uint8_t *buffer = (uint8_t *)malloc(IMAGES*8*128); // 8 page 128 pixel
	if (buffer == NULL) {
		ESP_LOGE(TAG, "malloc failed");
		while(1) { vTaskDelay(1); }
	}

	// Convert from segmentDisplay to segmentImage
	int index = 0;
	for (int i=0;i<IMAGES;i++) {
		ssd1306_clear_screen(&dev, true);
		ssd1306_bitmaps(&dev, 31, 0, mouse[i], 64, 64, false);
		vTaskDelay(200 / portTICK_PERIOD_MS);
		ssd1306_get_buffer(&dev, &buffer[index]);
		index = index + 1024;
	}

	// Create queue
	app_event_hid = xQueueCreate(10, sizeof(HID_EVENT_t));
	configASSERT( app_event_hid );

	// Start tasks
	xTaskCreate(&usb_hid_task, "usb_hid_task", 1024*2, NULL, 9, NULL);

	ssd1306_set_buffer(&dev, &buffer[0]);
	ssd1306_show_buffer(&dev);

	// Wait event
	HID_EVENT_t hidEvent;
	while(1) {
		BaseType_t received = xQueueReceive(app_event_hid, &hidEvent, portMAX_DELAY);
		ESP_LOGI(TAG, "xQueueReceive received=%d hidEvent.hid_event_type=%d", received, hidEvent.hid_event_type);
		if (hidEvent.hid_event_type == APP_EVENT_MOUSE) {
			ESP_LOGI(TAG, "mouse_event.button1=%d mouse_event.button2=%d mouse_event.button3=%d",
				hidEvent.mouse_event.button1, hidEvent.mouse_event.button2, hidEvent.mouse_event.button3);
			if (hidEvent.mouse_event.button1 == 0 && hidEvent.mouse_event.button2 == 0 && hidEvent.mouse_event.button3 == 0) {
				ssd1306_set_buffer(&dev, &buffer[0]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 1 && hidEvent.mouse_event.button2 == 0 && hidEvent.mouse_event.button3 == 0) {
				ssd1306_set_buffer(&dev, &buffer[1024]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 0 && hidEvent.mouse_event.button2 == 1 && hidEvent.mouse_event.button3 == 0) {
				ssd1306_set_buffer(&dev, &buffer[2048]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 0 && hidEvent.mouse_event.button2 == 0 && hidEvent.mouse_event.button3 == 1) {
				ssd1306_set_buffer(&dev, &buffer[3072]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 1 && hidEvent.mouse_event.button2 == 1 && hidEvent.mouse_event.button3 == 0) {
				ssd1306_set_buffer(&dev, &buffer[4096]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 1 && hidEvent.mouse_event.button2 == 0 && hidEvent.mouse_event.button3 == 1) {
				ssd1306_set_buffer(&dev, &buffer[5120]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 0 && hidEvent.mouse_event.button2 == 1 && hidEvent.mouse_event.button3 == 1) {
				ssd1306_set_buffer(&dev, &buffer[6144]);
				ssd1306_show_buffer(&dev);
			} else if (hidEvent.mouse_event.button1 == 1 && hidEvent.mouse_event.button2 == 1 && hidEvent.mouse_event.button3 == 1) {
				ssd1306_set_buffer(&dev, &buffer[7168]);
				ssd1306_show_buffer(&dev);
			}
		}
	}
}