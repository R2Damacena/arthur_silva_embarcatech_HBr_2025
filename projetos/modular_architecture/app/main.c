#include <stdio.h>
#include "pico/stdlib.h"
#include "hal_led.h"

int main() {
    stdio_init_all();
    
    if (hal_led_init()) {
        printf("Falha ao inicializar o LED\n");
        return -1;
    }

    while (true) {
        hal_led_toggle();
        sleep_ms(500);
    }
}