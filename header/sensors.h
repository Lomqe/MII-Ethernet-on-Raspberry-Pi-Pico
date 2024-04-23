#pragma once
#include "pico/stdlib.h"

// FLAG to pace both cores
volatile static bool sampling = true;  

#define ADC_PIN_LEVEL 3.3   // ADC max value in Volts
#define ADC_12B_LEVEL 4095  // ADC max 12 bit value

static struct sensor{
    uint adc_input;         // Input to ADC 
    uint8_t size;           // Size of the buffers
    float sample_val;       // Last sampled valued by ADC

    uint8_t* lut_cm;        // Pointer to buffer with datasheet values
    float* lut_volt;        // Pointer to buffer with datasheet values

    float max_val;
    float min_val;
}sensor1, sensor2;

#define SENSOR1_MAX_VAL sensor1_lut_volt[0]
#define SENSOR1_MIN_VAL sensor1_lut_volt[17]
// Sensors 10-80 prox. LUT from datasheet 
static float sensor1_lut_volt[] = {2.45, 2.11, 1.87, 1.65, 1.51, 1.39, 1.13, 0.98, 0.85, 0.75, 0.68, 0.61, 0.58, 0.52, 0.50, 0.47, 0.43, 0.41};
static uint8_t sensor1_lut_cm[] = {10, 12, 14, 16, 18, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80};

#define SENSOR2_MAX_VAL sensor2_lut_volt[0]
#define SENSOR2_MIN_VAL sensor2_lut_volt[13]
// Sensors 20-150 prox. LUT from datasheet  
static float sensor2_lut_volt[] = {2.55, 1.99, 1.52, 1.25, 1.05, 0.90, 0.80, 0.73, 0.67, 0.60, 0.55, 0.50, 0.49, 0.48};
static uint8_t sensor2_lut_cm[] = {20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


uint8_t sensor_sample(struct sensor* s);
void sensors_init();