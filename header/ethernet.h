#pragma once
#include "pico/stdlib.h"
#include "hardware/dma.h"

// The default PHY address is 00001 and may vary from 1-7 (0 = broadcast)
static const uint8_t phy_address = 1;
static const uint32_t ethernet_polynom = 0xEDB88320U;

static uint ethernet_frame_crc2(const uint32_t *data, int length);
static uint ethernet_frame_crc(const uint8_t *data, int length);
void mii_ethernet_output(uint8_t* tx_buffer, int length);
void mii_ethernet_output_opt(uint8_t* tx_buffer, int length);
void mii_ethernet_output_opt2(uint32_t* tx_buffer, int length);