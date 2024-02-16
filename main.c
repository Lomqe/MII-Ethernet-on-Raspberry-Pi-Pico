#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"

#include "defines.h"
#include "miim.h"

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

  // Set speed to 10 Mbps
  mii_mdio_write(phy_address, 0, 0x0);

  // Blink test
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  while (true) {
        for(uint16_t i = 0; i < 5; ++i){
          gpio_put(PICO_DEFAULT_LED_PIN, 1);
          sleep_ms(250);
          gpio_put(PICO_DEFAULT_LED_PIN, 0);
          sleep_ms(250);
        }
        for(uint16_t i = 0; i < 8; ++i){
          gpio_put(PICO_DEFAULT_LED_PIN, 1);
          sleep_ms(100);
          gpio_put(PICO_DEFAULT_LED_PIN, 0);
          sleep_ms(80);
        }

        // test resetu KSZ
        // sleep_ms(15000);
        // gpio_put(KSZ_RST, true);
        // sleep_ms(10000);
        // gpio_put(KSZ_RST, false);
        
    }
}
