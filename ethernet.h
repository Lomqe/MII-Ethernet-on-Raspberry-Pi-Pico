#pragma once

static uint ethernet_frame_crc2(const uint32_t *data, int length);
static uint ethernet_frame_crc(const uint8_t *data, int length);
static void mii_ethernet_output(uint8_t* tx_buffer, int length);
static void mii_ethernet_output_opt(uint8_t* tx_buffer, int length);
static void mii_ethernet_output_opt2(uint32_t* tx_buffer, int length);
static void write_sensor_data(uint8_t dist1, uint8_t dist2);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

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

static void mii_ethernet_output(uint8_t* tx_buffer, int length){
    if (length < 60){
        // pad
        length = 60;
    }

    uint crc = ethernet_frame_crc(tx_buffer, length);

    // printf("TX: ");
    // for (int i = 0; i < length; ++i) {
    //     printf("%02X", tx_buffer[i]);
    // }
    // printf("\n");

    dma_channel_wait_for_finish_blocking(tx_dma);
    
    //pio_set_irq0_source_enabled(pio0, pis_interrupt0, true); // asi nebude potreba 
    // irq > clear > reset SM na zacatek

    //while(pio_interrupt_get(pio0, 0)) tight_loop_contents();
    //pio_sm_restart(pio0, sm_tx);
   
    int index = 0;

    // PREAMBLE
    //  D0   D1   D2   D3   TXE 
    //  [1]  [0]  [1]  [0]  [1]
    for (int i = 0; i < 15; ++i){
        tx_frame_bits[index++] = 0x15;
    }

    // SFD
    //  D0   D1   D2   D3   TXE 
    //  [1]  [0]  [1]  [1]  [1]
    for (int i = 0; i < 1; ++i){
        tx_frame_bits[index++] = 0x17;
    }

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
    for (int i = 0; i < (12 * 2); ++i){
        tx_frame_bits[index++] = 0x00;
    }
    
    dma_channel_configure(
        tx_dma, &tx_dma_config,
        &pio0->txf[sm_tx],
        tx_frame_bits,
        index,
        true
    );
}

static void mii_ethernet_output_opt(uint8_t* tx_buffer, int length){
    if (length < 60){
        // pad
        length = 60;
    }

    uint crc = ethernet_frame_crc(tx_buffer, length);
    
    while(!first_transm && !pio_interrupt_get(pio0, 0)) tight_loop_contents();
    if(pio_interrupt_get(pio0, 0)) pio_sm_restart(pio0, sm_tx);

    first_transm = 0;

    uint8_t index = 1;  // prvni je pro index - pozor bude potreba pote zmenit na vetsi typ!!!

    // DATA
    for (int i = 0; i < length; i++){
        uint8_t b = tx_buffer[i];                           
        tx_frame_bits[index++] =        ((b >> 3) & 0x11)    
                                      | ((b >> 1) & 0x22)
                                      | ((b << 1) & 0x44)
                                      | ((b << 3) & 0x88);
    }

    // CRC
    for (int i = 0; i < 4; i++){
        uint8_t b = ((uint8_t*)&crc)[i];
        tx_frame_bits[index++] =        ((b >> 3) & 0x11)
                                      | ((b >> 1) & 0x22)
                                      | ((b << 1) & 0x44)
                                      | ((b << 3) & 0x88);
    }

    // potreba snizit o dva jelikoz prvni byte je 
    // v pio programu odeslan bez odecteni scratch reg.
    // a jeden pull je pro samotnou hodnotu bytu
    tx_frame_bits[0] = index-2;

    // TODO - Zkusit pri startu DMA hodit CPU do sleep aby nehrabal na AHB
    //      - a probudit jej pres DMA IRQ (z dokumentace to vsak vypada, ze to neni mozne)
    dma_channel_configure(
        tx_dma, &tx_dma_config,
        &pio0->txf[sm_tx],
        tx_frame_bits,
        index,                 
        true
    );
}

// 32 bit version - LENGTH HAS TO BE DIVISIBLE BY 4 !!!
static void mii_ethernet_output_opt2(uint32_t* tx_buffer, int length){
    if ((length) < 15){
        // pad
        length = 15;
    }

    uint crc = ethernet_frame_crc2(tx_buffer, length);
    
    dma_channel_wait_for_finish_blocking(tx_dma);

    while(!first_transm && !pio_interrupt_get(pio0, 0)) tight_loop_contents();
    if(pio_interrupt_get(pio0, 0)) pio_sm_restart(pio0, sm_tx);

    first_transm = 0;
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

static void write_sensor_data(uint8_t dist1, uint8_t dist2){
    uint8_t val1[4] = {0x0, 0x0, 0x0, 0x0};
    uint8_t val2[4] = {0x0, 0x0, 0x0, 0x0};

    itoa(dist1, val1, 10);
    itoa(dist2, val2, 10);

    for(uint8_t i = 0; i < 3; ++i){
      ethernet_packet[55+i] = val1[i]; 
      ethernet_packet[73+i] = val2[i];
    }
}