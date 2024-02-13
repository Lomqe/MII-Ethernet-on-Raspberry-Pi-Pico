#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"

#include "defines.h"

int main(void){
  set_sys_clock_khz(SYSTEM_FREQ_KHZ, true);
  stdio_init_all();

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_put(PICO_DEFAULT_LED_PIN, 1);
  while(1){

  }

}
