#pragma once

// test function "something to do..."
void power_test(){
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
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
}

#define TEST_PIN 22u
// Test impulse, intended to be used when TXEN is set
void test_impulse(){
  gpio_set_dir(TEST_PIN, true);

  gpio_put(TEST_PIN, true);
  gpio_put(TEST_PIN, false);
}