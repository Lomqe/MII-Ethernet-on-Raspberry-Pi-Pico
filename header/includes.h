#pragma once

// DEFAULT
#include <stdio.h>

// RPP
#include "hardware/adc.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/structs/xip_ctrl.h"  

// PROJECT
#include "../header/defines.h"
#include "../header/miim.h"
#include "../header/test.h"
#include "../header/ethernet.h"
#include "../header/sensors.h"

// PIOASM
#include "mii_tx.pio.h"
#include "mii_rx.pio.h"