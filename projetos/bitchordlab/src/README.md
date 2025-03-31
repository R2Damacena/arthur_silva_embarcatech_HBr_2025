# BitChordLab

BitChordLab é um sistema embarcado interativo, desenvolvido como projeto final no programa de capacitação EmbarcaTech, projetado para auxiliar no aprendizado de acordes de instrumentos de cordas.

## Objetivo

Criar uma ferramenta visual e interativa que ajuda estudantes de música a visualizar e memorizar acordes de:
- Violão
- Charango

Utilizando LEDs coloridos e um display informativo para guiar o aprendizado.

## Lista de materiais

| Componente            | Conexão na Raspberry Pi Pico |
|-----------------------|------------------------------|
| BitDogLab (RP2040)     | -                            |
| Matriz WS2818B 5x5    | GPIO7                        |
| Display OLED I2C      | SDA: GPIO14 / SCL: GPIO15    |
| Joystick              | VRX: GPIO27 / VRY: GPIO26 / SW: GPIO22 |
| Botão A               | GPIO5                        |
| Botão B               | GPIO6                        |

## Execução

1. Abra o projeto no VS Code com o ambiente de desenvolvimento configurado para Raspberry Pi Pico
2. Compile o projeto usando CMAKE (build)
3. Conecte a Pico via USB em modo de boot (pressione o botão BOOTSEL enquanto conecta)
4. Copie o arquivo .uf2 gerado (da pasta build) para a unidade RPI-RP2
5. O dispositivo será reiniciado e executará o BitChordLab automaticamente

## Funcionalidades

- **Menu de seleção de instrumento**: Escolha entre Violão e Charango
- **Visualização de acordes**: Cada acorde é representado por pontos de LED na matriz 5x5
- **Navegação por acordes**: Use o joystick (esquerda/direita) ou botão B para trocar entre acordes
- **Exibição de informações**: O display OLED mostra o nome do acorde atual
- **Interface intuitiva**: Navegue facilmente entre os instrumentos usando o joystick (cima/baixo)
- **Retorno ao menu**: Pressione o botão A a qualquer momento para voltar ao menu principal

## 🖼️ Imagens do Projeto

### Matriz de LEDs durante execução
![acorde_led](./assets/acorde.jpg)

## Acordes disponíveis

### Violão
- C Maior
- D Maior
- D Menor
- E Maior
- E Menor
- A Maior
- A Menor

### Charango
- C Maior
- D Maior
- E Maior
- F Maior
- G Maior
- A Maior
- B Maior

## Controles

- **Joystick para cima/baixo**: Navegar no menu de seleção de instrumento
- **Joystick para esquerda/direita**: Alternar entre acordes
- **Botão A**: Voltar ao menu principal
- **Botão B**: Avançar para o próximo acorde

## Estrutura do código

O projeto foi desenvolvido em C utilizando o SDK do Raspberry Pi Pico, com integração PIO para controlar os LEDs WS2818B e I2C para o display OLED.

---

## Licença
GPL-3.0