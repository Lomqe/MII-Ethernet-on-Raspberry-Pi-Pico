#pragma once
#include "pico/stdlib.h"

static void mii_mdio_out(bool bit);
static bool mii_mdio_in(void);
void mii_mdio_write(uint addr, uint reg, int val);
uint16_t mii_mdio_read(uint addr, uint reg);