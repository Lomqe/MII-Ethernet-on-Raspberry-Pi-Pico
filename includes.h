#pragma once

// DEFAULT
#include <stdio.h>

// RPP
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/adc.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"

// PROJECT
#include "defines.h"
#include "miim.h"
#include "test.h"
#include "ethernet.h"

// PIOASM
#include "mii_tx.pio.h"