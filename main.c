#include "includes.h"

// Comment to set 100 Mbps
#define _10_MBPS

int main(void){
  // De-assert reset
  gpio_init(KSZ_RST);
  gpio_set_dir(KSZ_RST, GPIO_OUT);
  gpio_put(KSZ_RST, false);

  // Change sys_clk to 100 MHz
  set_sys_clock_khz(SYSTEM_FREQ_100000_KHZ, true);
  stdio_init_all();
  
  sleep_ms(5000);

  // PIO

  sm_tx = pio_claim_unused_sm(pio0, true);

  #ifdef _10_MBPS
  // Set speed to 10 Mbps
  mii_mdio_write(phy_address, 0, 0x0);
  uint pio0_offset = pio_add_program(pio0, &mii_10mhz_tx_program);
  mii_10mhz_tx_init(pio0, sm_tx, pio0_offset, KSZ_TXEN);

  #else
  // Set speed to 100 Mbps
  mii_mdio_write(phy_address, 0, 0x2000);
  uint pio0_offset = pio_add_program(pio0, &mii_100mhz_tx_program);
  mii_100mhz_tx_init(pio0, sm_tx, pio0_offset, KSZ_TXEN);
  #endif

  // DMA

  tx_dma = dma_claim_unused_channel(true);
  tx_dma_config = dma_channel_get_default_config(tx_dma);

  channel_config_set_transfer_data_size(&tx_dma_config, DMA_SIZE_8);
  channel_config_set_write_increment(&tx_dma_config, false);
  channel_config_set_read_increment(&tx_dma_config, true);
  channel_config_set_dreq(&tx_dma_config, pio_get_dreq(pio0, sm_tx, true));

  // TX 

  uint16_t link = 0;
  while(true){
    //power_test();
    // Wait so the network wont get congested
    sleep_ms(1000);
    printf("Control register = %02X\n", link = mii_mdio_read(phy_address, 0));
    // Re-sending a unique packet
    mii_ethernet_output(netcat_packet, NCAT_PACKET_LENGTH);
    printf("dma proc...\n");}
}
