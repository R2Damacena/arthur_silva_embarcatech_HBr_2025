#ifndef LED_EMBUTIDO_H
#define LED_EMBUTIDO_H

/**
 * @brief Inicializa o LED embutido da Raspberry Pi Pico W
 * 
 * @return int 0 se bem-sucedido, -1 caso contrário
 */
int led_embutido_init(void);

/**
 * @brief Define o estado do LED embutido
 * 
 * @param estado 1 para ligado, 0 para desligado
 */
void led_embutido_set(int estado);

/**
 * @brief Obtém o estado atual do LED embutido
 * 
 * @return int 1 se ligado, 0 se desligado
 */
int led_embutido_get(void);

#endif // LED_EMBUTIDO_H