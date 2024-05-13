#include <stdlib.h>
#include <stdio.h>

#include "../header/ethernet.h"
#include "../header/defines.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"

static uint ethernet_frame_crc(const uint8_t *data, int length){
    uint crc = 0xFFFFFFFF;  /* Initial value */
    while(--length >= 0){
        uint8_t current_octet = *data++;
        for (int bit = 8; --bit >= 0; current_octet >>= 1){
          if ((crc ^ current_octet) & 1){
            crc >>= 1;
            crc ^= ethernet_polynom;
          }else
            crc >>= 1;
        }
    }
    return ~crc;
}

static uint ethernet_frame_crc2(const uint32_t *data, int length){
    uint crc = 0xFFFFFFFF;  /* Initial value */
    while(--length >= 0){
        uint32_t current = *data++;
        for (int bit = 32; --bit >= 0; current >>= 1){
          if ((crc ^ current) & 1){
            crc >>= 1;
            crc ^= ethernet_polynom;
          }else
            crc >>= 1;
        }
    }
    return ~crc;
}

void mii_ethernet_output(uint8_t* tx_buffer, int length){
    // pad
    if (length < 60)
        length = 60;

    uint crc = ethernet_frame_crc(tx_buffer, length);

    dma_channel_wait_for_finish_blocking(tx_dma);
   
    int index = 0;

    // PREAMBLE
    // D0   D1   D2   D3   TXE 
    // [1]  [0]  [1]  [0]  [1]
    for (int i = 0; i < 15; ++i)
        tx_frame_bits[index++] = 0x15;


    // SFD
    // D0   D1   D2   D3   TXE 
    // [1]  [0]  [1]  [1]  [1]
    for (int i = 0; i < 1; ++i)
        tx_frame_bits[index++] = 0x17;
    
    // DATA
    for (int i = 0; i < length; ++i){
        uint8_t b = tx_buffer[i];
        tx_frame_bits[index++] = 0x01 | ((b >> 2) & 0x02) 
                                      | (b & 0x04)
                                      | ((b << 2) & 0x08)
                                      | ((b << 4) & 0x10);
        b >>= 4;
        tx_frame_bits[index++] = 0x01 | ((b >> 2) & 0x02) 
                                      | (b & 0x04)
                                      | ((b << 2) & 0x08)
                                      | ((b << 4) & 0x10);
    }

    // CRC
    for (int i = 0; i < 4; ++i){
        uint8_t b = ((uint8_t*)&crc)[i];
        tx_frame_bits[index++] = 0x01 | ((b >> 2) & 0x02) 
                                      | (b & 0x04)
                                      | ((b << 2) & 0x08)
                                      | ((b << 4) & 0x10);
        b >>= 4;
        tx_frame_bits[index++] = 0x01 | ((b >> 2) & 0x02)
                                      | (b & 0x04)
                                      | ((b << 2) & 0x08)
                                      | ((b << 4) & 0x10);
    }
    
    // IDLE
    for (int i = 0; i < (12 * 2); ++i)
        tx_frame_bits[index++] = 0x00;
    
    dma_channel_configure(
        tx_dma, &tx_dma_config,
        &pio0->txf[sm_tx],
        tx_frame_bits,
        index,
        true
    );
}

void mii_ethernet_output_opt(uint8_t* tx_buffer, int length){
    // pad
    if (length < 60)
        length = 60;
    
    uint crc = ethernet_frame_crc(tx_buffer, length);

    uint8_t index = 1;  // Reserve first value for index after we feed in the data
                        // If the packet exceeds length of 255 there is a need
                        // to change the uint8_t type and topology

    // DATA
    for (int i = 0; i < length; i++){
        uint8_t b = tx_buffer[i];                           
        tx_frame[index++] =             ((b >> 3) & 0x11)    
                                      | ((b >> 1) & 0x22)
                                      | ((b << 1) & 0x44)
                                      | ((b << 3) & 0x88);
    }

    // CRC
    for (int i = 0; i < 4; i++){
        uint8_t b = ((uint8_t*)&crc)[i];
        tx_frame[index++] =             ((b >> 3) & 0x11)
                                      | ((b >> 1) & 0x22)
                                      | ((b << 1) & 0x44)
                                      | ((b << 3) & 0x88);
    }

    // Decrement by two since first byte is sent before
    // Decrementing loop counter and one byte is the 
    // counter value itself
    tx_frame[0] = index-2;
    
    dma_channel_configure(
        tx_dma, &tx_dma_config,
      //  tx_dma, c,
        &pio0->txf[sm_tx],
        tx_frame,
        index,                 
        true
    );

    printf("config_dma = %X\n", &tx_dma_config); 

}

// 32 bit version - LENGTH HAS TO BE DIVISIBLE BY 4 !!!
void mii_ethernet_output_opt2(uint32_t* tx_buffer, int length){
    // pad
    if ((length) < 15)
        length = 15;

    uint crc = ethernet_frame_crc2(tx_buffer, length);
    
    dma_channel_wait_for_finish_blocking(tx_dma);

    uint32_t index = 1;  // prvni je pro index

    // DATA
    for (int i = 0; i < length; i++){
        uint32_t b = tx_buffer[i];                           
        tx_frame_32b[index++] =         ((b >> 3) & 0x11111111)    
                                      | ((b >> 1) & 0x22222222)
                                      | ((b << 1) & 0x44444444)
                                      | ((b << 3) & 0x88888888);
    }

    // CRC
    tx_frame_32b[index++] =             ((crc >> 3) & 0x11111111)
                                      | ((crc >> 1) & 0x22222222)
                                      | ((crc << 1) & 0x44444444)
                                      | ((crc << 3) & 0x88888888);

    // Decrement because index is not part of data and 1 transfer 
    // is executed before starting the loop counter
    tx_frame_32b[0] = ((index-1)*8)-1;
      
    dma_channel_configure(
        tx_dma, &tx_dma_config,
        &pio0->txf[sm_tx],
        tx_frame_32b,
        index,                 
        true
    );
}