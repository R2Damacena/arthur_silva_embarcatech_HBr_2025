#ifndef HAL_LED_H
#define HAL_LED_H

/**
 * @brief Inicializa a camada de abstração do LED
 * 
 * @return int 0 se bem-sucedido, -1 caso contrário
 */
int hal_led_init(void);

/**
 * @brief Acende o LED
 */
void hal_led_on(void);

/**
 * @brief Apaga o LED
 */
void hal_led_off(void);

/**
 * @brief Alterna o estado do LED (toggle)
 */
void hal_led_toggle(void);

#endif // HAL_LED_H