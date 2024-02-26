#pragma once

static uint16_t ethernet_frame_crc(const uint8_t *data, int length);
static void mii_ethernet_output(uint8_t* tx_buffer, uint length);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

static uint16_t ethernet_frame_crc(const uint8_t *data, int length){
    uint16_t crc = 0;  /* Initial value */
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
    // pro ncat packet je CRC = 0xF203 a ~crc = 0x0DFC
    return ~crc;
}

static void mii_ethernet_output(uint8_t* tx_buffer, uint length){
    if (length < 60){
        // pad
        length = 60;
    }

    uint crc = ethernet_frame_crc(tx_buffer, length);

    // printf("TX: ");
    // for (int i = 0; i < length; i++) {
    //     printf("%02X", tx_frame[i]);
    // }
    // printf("\n");

    dma_channel_wait_for_finish_blocking(tx_dma);

    int index = 0;

    // PREAMBLE
    //  TXE  D3   D2   D1   D0 
    //  [1]  [0]  [1]  [0]  [1]
    for (int i = 0; i < 15; i++){
        tx_frame_bits[index++] = 0x15;
    }

    // SFD
    //  TXE  D3   D2   D1   D0 
    //  [1]  [1]  [1]  [0]  [1]
    for (int i = 0; i < 1; i++){
        tx_frame_bits[index++] = 0x1D;
    }

    // DATA
    for (int i = 0; i < length; i++){
        uint8_t b = tx_buffer[i];
        tx_frame_bits[index++] = 0x10 | ((b >> 0) & 0x0F);
        tx_frame_bits[index++] = 0x10 | ((b >> 4) & 0x0F);
    }

    // CRC
    for (int i = 0; i < 2; i++){
        uint8_t b = ((uint8_t*)&crc)[i];
        tx_frame_bits[index++] = 0x10 | ((b >> 0) & 0x0F);
        tx_frame_bits[index++] = 0x10 | ((b >> 4) & 0x0F);
    }
    
    // IDLE
    for (int i = 0; i < (12 * 2); i++){
        tx_frame_bits[index++] = 0x00;
    }

    dma_channel_configure(
        tx_dma, &tx_dma_config,
        tx_frame,//pio_0->rxf[sm0] + 3,      // TODO - NADEFINOVAT PIO INSTANCI (zatim jen aby nehodilo error)
        tx_frame_bits,
        index,
        false
    );
}