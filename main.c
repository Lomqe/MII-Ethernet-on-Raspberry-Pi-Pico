#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"

#include "defines.h"

int main(void){

  // Reset signál KSZ na 0
  gpio_init(RST_KSZ);
  gpio_set_dir(RST_KSZ, GPIO_OUT);
  gpio_put(RST_KSZ, false);

  // Změna systém CLK na 100 MHZ
  set_sys_clock_khz(SYSTEM_FREQ_100000_KHZ, true);
  stdio_init_all();

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  uint32_t actual = clock_get_hz(clk_sys);
  printf("System clock is set to: %lu Hz\n", actual);

  // TODO - nakonfigurovat 10 Mbps

  // Testovací program na blikání LED
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
        sleep_ms(15000);
        gpio_put(RST_KSZ, true);
        sleep_ms(10000);
        gpio_put(RST_KSZ, false);
    }
}
