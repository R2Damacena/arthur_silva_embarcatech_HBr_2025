#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

float adc_to_celsius(uint16_t adc_val) {
    const float conversion_factor = 3.3f / (1 << 12); // 3.3V / 4096
    float voltage = adc_val * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

int main(void){
    stdio_init_all();

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while(1){
        uint16_t raw = adc_read();
        float temp = adc_to_celsius(raw);
        printf("Temp = %f C\n", temp);
        sleep_ms(1000);
    }
}