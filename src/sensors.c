#include <stdio.h>

#include "../header/sensors.h"
#include "hardware/adc.h"

uint8_t sensor_sample(struct sensor* s){
    uint left_index = 0;
    adc_select_input(s->adc_input);
    s->sample_val = (float)adc_read()/ADC_12B_LEVEL*ADC_PIN_LEVEL;

    printf("sample val = %.2f ", s->sample_val);

    // Saturate min & max
    if(s->sample_val > s->max_val) s->sample_val = s->max_val;
    else if(s->sample_val < s->min_val) s->sample_val = s->min_val;

    uint right_index = 1;
    for(; right_index < s->size; ++right_index){ 
        if(!(s->sample_val < s->lut_volt[right_index])){
            left_index = right_index-1;
        break;
        }
    }

    //                example for values between 50 - 60 cm
    //                60   - (60-50)     * (val-1.05) / (1.25-1.05)
    //                i[2] - (i[2]-i[1]) * (val-i[2]) / (i[1]-i[2]) 
    return (uint8_t)((float)s->lut_cm[right_index] - ((float)s->lut_cm[right_index]-(float)s->lut_cm[left_index]) * 
          (s->sample_val-s->lut_volt[right_index])/(s->lut_volt[left_index]-s->lut_volt[right_index]));
}

void sensors_init(){
    sensor1.adc_input = 0;
    sensor1.lut_cm = sensor1_lut_cm;
    sensor1.lut_volt = sensor1_lut_volt;
    sensor1.size = sizeof(sensor1_lut_cm)/sizeof(sensor1_lut_cm[0]);
    sensor1.max_val = SENSOR1_MAX_VAL;
    sensor1.min_val = SENSOR1_MIN_VAL;

    sensor2.adc_input = 1;
    sensor2.lut_cm = sensor2_lut_cm;
    sensor2.lut_volt = sensor2_lut_volt;
    sensor2.size = sizeof(sensor2_lut_cm)/sizeof(sensor2_lut_cm[0]);
    sensor2.max_val = SENSOR2_MAX_VAL;
    sensor2.min_val = SENSOR2_MIN_VAL;
}