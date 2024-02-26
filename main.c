#include "includes.h"

int main(void){
  // De-assert reset
  gpio_init(KSZ_RST);
  gpio_set_dir(KSZ_RST, GPIO_OUT);
  gpio_put(KSZ_RST, false);

  // Change sys_clk to 100 MHz
  set_sys_clock_khz(SYSTEM_FREQ_100000_KHZ, true);
  stdio_init_all();

  uint32_t actual = clock_get_hz(clk_sys);
  printf("System clock is set to: %lu Hz\n", actual);

  // Set speed to 100 Mbps
  //mii_mdio_write(phy_address, 0, 0x2000);
  
  // Set speed to 10 Mbps
  mii_mdio_write(phy_address, 0, 0x0);

  // DMA

  tx_dma = dma_claim_unused_channel(true);
  tx_dma_config = dma_channel_get_default_config(tx_dma);

  // TX 

  uint16_t link = 0;
  while(true){
    power_test();
    // Wait so the network wont get congested
    sleep_ms(1);
    printf("Control register = %02X\n", link = mii_mdio_read(phy_address, 0));
    // Re-sending a unique packet
    mii_ethernet_output(netcat_packet, NCAT_PACKET_LENGTH);
  }
}
