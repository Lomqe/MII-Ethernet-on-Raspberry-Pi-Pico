#include <stdio.h>
#include <stdlib.h>

#include "../header/defines.h"
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
    return (uint8_t)((float)s->lut_cm[right_index]-((float)s->lut_cm[right_index]-(float)s->lut_cm[left_index]) * 
          (s->sample_val-s->lut_volt[right_index])/(s->lut_volt[left_index]-s->lut_volt[right_index]));
}

struct sensor sensors_init(uint8_t number){   
    struct sensor s;

    if(number){
        s.adc_input = number;
        s.lut_cm = sensor2_lut_cm;
        s.lut_volt = sensor2_lut_volt;
        s.size = sizeof(sensor2_lut_cm)/sizeof(sensor2_lut_cm[0]);
        s.max_val = SENSOR2_MAX_VAL;
        s.min_val = SENSOR2_MIN_VAL;
        
    }
    else{
        s.adc_input = number;
        s.lut_cm = sensor1_lut_cm;
        s.lut_volt = sensor1_lut_volt;
        s.size = sizeof(sensor1_lut_cm)/sizeof(sensor1_lut_cm[0]);
        s.max_val = SENSOR1_MAX_VAL;
        s.min_val = SENSOR1_MIN_VAL;
    }

    return s;
}

void write_sensor_data(uint8_t dist1, uint8_t dist2, uint32_t packet_count, uint8_t *packet){
    uint8_t val1[4] = {0x0, 0x0, 0x0, 0x0};
    uint8_t val2[4] = {0x0, 0x0, 0x0, 0x0};
    uint8_t val3[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    itoa(dist1, val1, 10);
    itoa(dist2, val2, 10);
    itoa(packet_count, val3, 10);

    for(uint8_t i = 0; i < 6; ++i)
        packet[57+i] = val3[i];

    for(uint8_t i = 0; i < 3; ++i){
      packet[76+i] = val1[i]; 
      packet[95+i] = val2[i];
    }
}