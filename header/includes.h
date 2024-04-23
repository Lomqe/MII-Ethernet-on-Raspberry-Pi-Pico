#pragma once

// DEFAULT
#include <stdio.h>

// RPP
#include "hardware/adc.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/structs/xip_ctrl.h"  

// PROJECT
#include "defines.h"
#include "miim.h"
#include "test.h"
#include "ethernet.h"
#include "sensors.h"

// PIOASM
#include "mii_tx.pio.h"
#include "mii_rx.pio.h"