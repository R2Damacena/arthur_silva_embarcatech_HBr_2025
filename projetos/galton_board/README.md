
# Galton Board Digital

Este projeto implementa uma versão digital de uma Galton Board (ou Plinko), demonstrando como uma série de decisões binárias aleatórias leva a uma **distribuição normal de probabilidades**.

## Objetivo

Criar uma visualização interativa que simula o comportamento de uma Galton Board usando:

- Display OLED
- Botões para incluir desequilíbrios ao modelo

##  Lista de materiais: 

| Componente            | Conexão na BitDogLab      |
|-----------------------|---------------------------|
| BitDogLab (RP2040)    | -                         |
| Matriz WS2812B 5x5    | GPIO7                     |
| Display OLED I2C      | SDA: GPIO14 / SCL: GPIO15 |

## Execução

1. Abra o projeto no VS Code, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VS Code ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código;
6. O histograma será atualizado no display OLED e os eventos simulados aparecerão na matriz de LEDs.

## Lógica

- A cada passo, a "bola" se desloca para esquerda ou direita com probabilidade 50%;
- O número total de passos define a profundidade do triângulo;
- A posição final da "bola" é contabilizada no vetor de contagem;
- As barras do OLED representam visualmente a frequência de cada posição.

##  Arquivos

- `src/...`: Código principal do projeto;
- `assets/galton_led.jpg`: Caminhos simulados;
- `assets/galton_hist.jpg`: Histograma de distribuição.

## 🖼️ Imagens do Projeto

### Matriz de LEDs durante execução
![galton_led](./assets/galton_led.jpg)

### Histograma no OLED
![galton_hist](./assets/galton_hist.jpg)

---

## 📜 Licença
GPL-3.0

