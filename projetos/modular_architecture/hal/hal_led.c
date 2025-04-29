#include "hal_led.h"
#include "led_embutido.h"

static int led_estado = 0;

int hal_led_init(void) {
    return led_embutido_init();
}

void hal_led_on(void) {
    led_estado = 1;
    led_embutido_set(led_estado);
}

void hal_led_off(void) {
    led_estado = 0;
    led_embutido_set(led_estado);
}

void hal_led_toggle(void) {
    led_estado = !led_estado;
    led_embutido_set(led_estado);
}