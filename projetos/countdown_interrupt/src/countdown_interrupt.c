#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
const uint BOTAO_A_PIN = 5;  // GPIO5 para iniciar/reiniciar a contagem
const uint BOTAO_B_PIN = 6;  // GPIO6 para contar cliques durante a contagem

// Variáveis globais
volatile int contador = 0;             // Contador regressivo
volatile int cliques_botao_b = 0;      // Contador de cliques do botão B
volatile bool contagem_ativa = false;  // Flag para controlar se a contagem está ativa
volatile bool atualizar_display = true; // Flag para indicar que o display precisa ser atualizado
volatile uint32_t ultimo_debounce_a = 0; // Para debounce do botão A
volatile uint32_t ultimo_debounce_b = 0; // Para debounce do botão B
const uint32_t DEBOUNCE_DELAY = 200 * 1000; // 200ms em microssegundos

// Handler de interrupção para o Botão A
void botao_a_callback(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = time_us_32();
    
    // Debounce para evitar múltiplos acionamentos
    if (tempo_atual - ultimo_debounce_a < DEBOUNCE_DELAY) {
        return;
    }
    ultimo_debounce_a = tempo_atual;
    
    // Reinicia a contagem
    contador = 9;
    cliques_botao_b = 0;
    contagem_ativa = true;
    atualizar_display = true;
}

// Handler de interrupção para o Botão B
void botao_b_callback(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = time_us_32();
    
    // Debounce para evitar múltiplos acionamentos
    if (tempo_atual - ultimo_debounce_b < DEBOUNCE_DELAY) {
        return;
    }
    ultimo_debounce_b = tempo_atual;
    
    // Incrementa o contador de cliques apenas se a contagem estiver ativa
    if (contagem_ativa && contador > 0) {
        cliques_botao_b++;
        atualizar_display = true;
    }
}

// Handler de interrupção comum para todos os GPIOs
void gpio_callback(uint gpio, uint32_t eventos) {
    if (gpio == BOTAO_A_PIN) {
        botao_a_callback(gpio, eventos);
    } else if (gpio == BOTAO_B_PIN) {
        botao_b_callback(gpio, eventos);
    }
}

int main()
{
    stdio_init_all(); // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    // Configuração dos botões
    gpio_init(BOTAO_A_PIN);
    gpio_init(BOTAO_B_PIN);
    gpio_set_dir(BOTAO_A_PIN, GPIO_IN);
    gpio_set_dir(BOTAO_B_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_A_PIN);
    gpio_pull_up(BOTAO_B_PIN);
    
    // Configuração das interrupções para os botões
    gpio_set_irq_enabled_with_callback(BOTAO_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BOTAO_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Exibir mensagem inicial
    ssd1306_draw_string(ssd, 5, 0, "Contador: 0");
    ssd1306_draw_string(ssd, 5, 16, "Cliques: 0");
    ssd1306_draw_string(ssd, 5, 32, "Pressione A para iniciar");
    render_on_display(ssd, &frame_area);

    // Variável para controlar o tempo da contagem regressiva
    absolute_time_t ultimo_segundo = get_absolute_time();
    
    while (true)
    {
        // Atualizar o display apenas quando necessário
        if (atualizar_display) {
            // Limpa o buffer de display
            memset(ssd, 0, ssd1306_buffer_length);
            
            // Prepara os textos a serem exibidos
            char contador_texto[20];
            char cliques_texto[20];
            char status_texto[30];
            
            sprintf(contador_texto, "Contador: %d", contador);
            sprintf(cliques_texto, "Cliques: %d", cliques_botao_b);
            
            if (contagem_ativa) {
                if (contador > 0) {
                    sprintf(status_texto, "Contagem ativa");
                } else {
                    sprintf(status_texto, "Pressione A para reiniciar");
                }
            } else {
                sprintf(status_texto, "Pressione A para iniciar");
            }
            
            // Desenha os textos no buffer
            ssd1306_draw_string(ssd, 5, 0, contador_texto);
            ssd1306_draw_string(ssd, 5, 16, cliques_texto);
            ssd1306_draw_string(ssd, 5, 32, status_texto);
            
            // Renderiza o buffer no display
            render_on_display(ssd, &frame_area);
            
            atualizar_display = false;
        }
        
        // Verifica se é hora de decrementar o contador (a cada segundo)
        if (contagem_ativa && contador > 0 && 
            absolute_time_diff_us(ultimo_segundo, get_absolute_time()) >= 1000000) {
            contador--;
            atualizar_display = true;
            ultimo_segundo = get_absolute_time();
            
            // Se o contador chegou a zero, desativa a contagem
            if (contador == 0) {
                contagem_ativa = false;
            }
        }
        
        // Pequeno delay para não sobrecarregar o processador
        sleep_ms(10);
    }

    return 0;
}