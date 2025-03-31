#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"

#define LED_COUNT 25
#define LED_PIN 7
#include "ws2818b.pio.h"

#define I2C_SDA 14
#define I2C_SCL 15

#define VRX 27
#define VRY 26
#define SW 22
#define ADC_CHANNEL_X 1
#define ADC_CHANNEL_Y 0

#define BUTTON_A 5
#define BUTTON_B 6

typedef enum
{
    MENU,
    VIOLAO,
    CHARANGO
} State;
State current_state = MENU;

struct pixel_t
{
    uint8_t G, R, B;
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t;

npLED_t leds[LED_COUNT];
PIO np_pio;
uint sm;
struct render_area frame_area;
uint selected_instrument = 0;
uint current_chord = 0;
uint16_t last_x = 2048;

char *instruments[] = {"Violão", "Charango"};
char *guitar_chords[] = {"C Maior", "D Maior", "D Menor", "E Maior", "E Menor", "A Maior", "A Menor"};
char *charango_chords[] = {"C Maior", "D Maior", "E Maior", "F Maior", "G Maior", "A Maior", "B Maior"};

const uint8_t guitar_patterns[][5] = {
    {21, 16, 14, 0, 0}, // C Maior
    {19, 11, 17, 0, 0}, // D Maior
    {20, 17, 11, 0, 0}, // D Menor
    {22, 15, 16, 0, 0}, // E Maior
    {15, 16, 0, 0, 0},  // E Menor
    {16, 17, 18, 0, 0}, // A Maior
    {21, 16, 17, 0, 0}  // A Menor
};

const uint8_t charango_patterns[][5] = {
    {11, 0, 0, 0, 0},    // C Maior
    {15, 16, 17, 0, 19}, // D Maior
    {24, 6, 18, 0, 0},   // E Maior
    {15, 22, 20, 0, 0},  // F Maior
    {16, 18, 12, 10, 0}, // G Maior
    {15, 23, 0, 0, 0},   // A Maior
    {5, 13, 19, 18, 17}  // B Maior
};

void npInit(uint pin);
void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b);
void npClear();
void npWrite();
void joystick_read_axis(uint16_t *eixo_x, uint16_t *eixo_y);
void update_display();
void update_leds();
void ssd1306_draw_filled_rect(uint8_t *ssd, int x, int y, int w, int h);

int main()
{
    stdio_init_all();

    npInit(LED_PIN);
    npClear();

    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();

    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&frame_area);

    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    while (true)
    {
        uint16_t eixo_x, eixo_y;
        joystick_read_axis(&eixo_x, &eixo_y);

        if (current_state == MENU)
        {
            if (eixo_y > 3000)
            {
                selected_instrument = 0;
                sleep_ms(200);
            }
            else if (eixo_y < 1000)
            {
                selected_instrument = 1;
                sleep_ms(200);
            }

            if (!gpio_get(BUTTON_B))
            {
                current_state = (selected_instrument == 0) ? VIOLAO : CHARANGO;
                current_chord = 0;
                sleep_ms(200);
            }
        }
        else
        {
            if (!gpio_get(BUTTON_A))
            {
                current_state = MENU;
                sleep_ms(200);
            }

            if (!gpio_get(BUTTON_B) || (eixo_x > 3500 && last_x <= 3500) || (eixo_x < 500 && last_x >= 500))
            {
                if (!gpio_get(BUTTON_B)) // Botão B pressionado -> Avança acorde
                {
                    current_chord = (current_chord + 1) % 7;
                }
                else if (eixo_x > 3500 && last_x <= 3500) // Joystick para direita -> Avança acorde
                {
                    current_chord = (current_chord + 1) % 7;
                }
                else if (eixo_x < 500 && last_x >= 500) // Joystick para esquerda -> Volta acorde
                {
                    current_chord = (current_chord - 1 + 7) % 7;
                }

                sleep_ms(200);
            }
        }

        last_x = eixo_x;
        update_display();
        update_leds();
        sleep_ms(100);
    }
}

void joystick_read_axis(uint16_t *eixo_x, uint16_t *eixo_y)
{
    adc_select_input(ADC_CHANNEL_X);
    sleep_us(2);
    *eixo_x = adc_read();

    adc_select_input(ADC_CHANNEL_Y);
    sleep_us(2);
    *eixo_y = adc_read();
}

void update_display()
{
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    if (current_state == MENU)
    {
        ssd1306_draw_string(ssd, 20, 10, "Instrumento:");

        if (selected_instrument == 0)
        {
            ssd1306_draw_filled_rect(ssd, 10, 24, 5, 10);
        }
        else
        {
            ssd1306_draw_filled_rect(ssd, 10, 40, 5, 10);
        }

        ssd1306_draw_string(ssd, 20, 24, "Violao");
        ssd1306_draw_string(ssd, 20, 40, "Charango");
    }
    else
    {
        char *chord_name = (current_state == VIOLAO) ? guitar_chords[current_chord] : charango_chords[current_chord];
        ssd1306_draw_string(ssd, 20, 24, chord_name);
    }

    render_on_display(ssd, &frame_area);
}

void update_leds()
{
    npClear();

    if (current_state == MENU)
    {
        npWrite();
        return;
    }

    const uint8_t(*chord_pattern)[5] = (current_state == VIOLAO) ? guitar_patterns : charango_patterns;

    for (int i = 0; i < 5; i++)
    {
        if (chord_pattern[current_chord][i] > 0 && chord_pattern[current_chord][i] < LED_COUNT)
        {
            npSetLED(chord_pattern[current_chord][i], 0, 1, 0);
        }
    }

    npWrite();
}

void ssd1306_draw_filled_rect(uint8_t *ssd, int x, int y, int w, int h)
{
    for (int i = x; i < x + w; i++)
    {
        for (int j = y; j < y + h; j++)
        {
            ssd1306_set_pixel(ssd, i, j, true);
        }
    }
}

void npInit(uint pin)
{
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, true);
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    npClear();
}
void npClear() { memset(leds, 0, sizeof(leds)); }
void npSetLED(const uint index, uint8_t r, uint8_t g, uint8_t b)
{

    r = ((r & 0xF0) >> 4) | ((r & 0x0F) << 4); // Troca os 4 MSB com os 4 LSB
    r = ((r & 0xCC) >> 2) | ((r & 0x33) << 2); // Troca pares de bits
    r = ((r & 0xAA) >> 1) | ((r & 0x55) << 1); // Troca bits individuais

    g = ((g & 0xF0) >> 4) | ((g & 0x0F) << 4); // Troca os 4 MSB com os 4 LSB
    g = ((g & 0xCC) >> 2) | ((g & 0x33) << 2); // Troca pares de bits
    g = ((g & 0xAA) >> 1) | ((g & 0x55) << 1); // Troca bits individuais

    b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4); // Troca os 4 MSB com os 4 LSB
    b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2); // Troca pares de bits
    b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1); // Troca bits individuais

    leds[index].R = r;
    leds[index].G = g;
    leds[index].B = b;
}
void npWrite()
{
    for (uint i = 0; i < LED_COUNT; i++)
    {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}
