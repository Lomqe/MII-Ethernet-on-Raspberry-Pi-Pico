//#define _10_MBPS    
//#define _100_MBPS   
//#define TX_OPT_2

#include "includes.h"


////////////////
// ADC Thread //
////////////////

void adc_main(void){
  adc_init();  
  adc_gpio_init(RPP_SENSOR1); 
  adc_gpio_init(RPP_SENSOR2);
  sensors_init();

  while(true){
    // Important when it comes to RELEASE mode
    dma_channel_wait_for_finish_blocking(tx_dma);

    sampling = 1;
    sleep_ms(250);

    //while(pio_interrupt_get(pio0, 0)) printf("PIO IRQ!\n\n");

    uint data1 = sensor_sample(&sensor1);
    uint data2 = sensor_sample(&sensor2);
    write_sensor_data(data1, data2);
    //write_sensor_data(sensor_sample(&sensor1), sensor_sample(&sensor2));
    sampling = 0;
  }
}


/////////////////
// Main Thread //
/////////////////

  int main(void){
  // De-assert reset
  gpio_init(KSZ_RST);
  gpio_set_dir(KSZ_RST, GPIO_OUT);
  gpio_put(KSZ_RST, false);

  // Change sys_clk
  // The bigger the VCO freq the lower the jitter it generates
  set_sys_clock_pll(VCO_FREQ_1500KHZ, 6, 2);
  
  stdio_init_all();
  sleep_ms(2000);
  printf("inicialized with CLK_SYS = %d MHz\n", clock_get_hz(clk_sys)/MHZ);

  // PIO 
  
  #ifdef _10_MBPS
  // Set speed to 10 Mbps
  mii_mdio_write(phy_address, KSZ_BASIC_CONTROL_REG, 0x0);
  uint pio0_offset = pio_add_program(pio0, &mii_10mhz_tx_program);
  mii_10mhz_tx_init(pio0, sm_tx, pio0_offset, KSZ_TXEN);
  #endif 

  #ifdef _100_MBPS
  // Set speed to 100 Mbps
  mii_mdio_write(phy_address, KSZ_BASIC_CONTROL_REG, 0x2000);
  uint pio0_offset = pio_add_program(pio0, &mii_100mhz_tx_program);
  mii_100mhz_tx_init(pio0, sm_tx, pio0_offset, KSZ_TXEN);
  #endif

  #ifdef TX_OPT_2
  mii_mdio_write(phy_address, KSZ_BASIC_CONTROL_REG, 0x2000);
  uint pio0_offset = pio_add_program(pio0, &mii_opt_tx2_program);
  mii_opt_tx2_init(pio0, sm_tx, pio0_offset, KSZ_TXEN);
  #else
  mii_mdio_write(phy_address, KSZ_BASIC_CONTROL_REG, 0x2000);
  uint pio0_offset = pio_add_program(pio0, &mii_opt_tx_program);
  mii_opt_tx_init(pio0, sm_tx, pio0_offset, KSZ_TXEN);
  #endif

  // DMA

  tx_dma = dma_claim_unused_channel(true);
  tx_dma_config = dma_channel_get_default_config(tx_dma);
  
  #ifdef TX_OPT_2
  channel_config_set_transfer_data_size(&tx_dma_config, DMA_SIZE_32);
  channel_config_set_bswap(&tx_dma_config, true);
  channel_config_set_write_increment(&tx_dma_config, false);
  channel_config_set_read_increment(&tx_dma_config, true);
  channel_config_set_dreq(&tx_dma_config, pio_get_dreq(pio0, sm_tx, true));
  #else
  channel_config_set_transfer_data_size(&tx_dma_config, DMA_SIZE_8);
  channel_config_set_write_increment(&tx_dma_config, false);
  channel_config_set_read_increment(&tx_dma_config, true);
  channel_config_set_dreq(&tx_dma_config, pio_get_dreq(pio0, sm_tx, true));
  #endif

  multicore_launch_core1(adc_main);
  printf("Control register = %02X\n", mii_mdio_read(phy_address, 0));
  
  while(true){    
    //if(gpio_get(KSZ_TXEN)) test_impulse();

    // Re-sending a unique packet
    while(!sampling){
      
      #ifdef TX_OPT_2
      int packet_length = sizeof(ethernet_packet2)/sizeof(uint32_t);
      mii_ethernet_output_opt2(ethernet_packet2, packet_length);
      #else
      int packet_length = sizeof(ethernet_packet)/sizeof(uint8_t);
      mii_ethernet_output_opt(ethernet_packet, packet_length);
      #endif

      printf("dma proc...\n");
    }
  }
}