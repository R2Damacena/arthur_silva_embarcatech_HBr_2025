#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

int main()
{
    stdio_init_all();

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do ADC para joystick
    adc_init();
    adc_gpio_init(26); // Y
    adc_gpio_init(27); // X

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display
    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1};

    calculate_render_area_buffer_length(&frame_area);

    // Buffer do display
    uint8_t ssd[ssd1306_buffer_length];

    while (true)
    {
        // Limpa o buffer do display
        memset(ssd, 0, ssd1306_buffer_length);

        // Leitura dos valores do joystick
        adc_select_input(0); // Y
        uint adc_y_raw = adc_read();
        adc_select_input(1); // X
        uint adc_x_raw = adc_read();

        // Mostra os valores no display
        char buf[32];
        snprintf(buf, sizeof(buf), "Joystick X: %4u", adc_x_raw);
        ssd1306_draw_string(ssd, 0, 0, buf);
        snprintf(buf, sizeof(buf), "Joystick Y: %4u", adc_y_raw);
        ssd1306_draw_string(ssd, 0, 10, buf);

        // Atualiza o display
        render_on_display(ssd, &frame_area);

        sleep_ms(100);
    }

    return 0;
}