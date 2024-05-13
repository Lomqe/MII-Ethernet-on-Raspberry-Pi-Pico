add_executable(mii src/main.c src/miim.c src/test.c src/sensors.c src/ethernet.c)

pico_generate_pio_header(mii ${CMAKE_CURRENT_LIST_DIR}/src/mii_tx.pio)
pico_generate_pio_header(mii ${CMAKE_CURRENT_LIST_DIR}/src/mii_rx.pio)


# pull in common dependencies
target_link_libraries(mii 
            pico_stdlib
            hardware_irq
            pico_multicore 
            hardware_pio
            hardware_adc
            hardware_dma
            )

# Pro debugger použít UART + propojit TX RX
pico_enable_stdio_usb(mii 0)
pico_enable_stdio_uart(mii 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(mii)

# add url via pico_set_program_url
example_auto_set_url(mii)
