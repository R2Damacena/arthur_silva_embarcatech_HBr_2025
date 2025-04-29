# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(modular_architecture C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(modular_architecture modular_architecture.c )

pico_set_program_name(modular_architecture "modular_architecture")
pico_set_program_version(modular_architecture "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(modular_architecture 0)
pico_enable_stdio_usb(modular_architecture 0)

# Add the standard library to the build
target_link_libraries(modular_architecture
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(modular_architecture PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(modular_architecture 
        
        )

pico_add_extra_outputs(modular_architecture)

# adiciona diretório de inclusão global
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# compila drivers como biblioteca
add_library(drivers STATIC
    drivers/led_embutido.c
)
target_link_libraries(drivers pico_stdlib pico_cyw43_arch_none)

# compila HAL como biblioteca
add_library(hal STATIC
    hal/hal_led.c
)
target_link_libraries(hal drivers)

# compila o aplicativo principal
add_executable(modular_app
    app/main.c
)
target_link_libraries(modular_app hal pico_stdlib)

# ativa saída USB e UART
pico_enable_stdio_usb(modular_app 1)
pico_enable_stdio_uart(modular_app 1)

# cria arquivos .uf2 adicionais etc
pico_add_extra_outputs(modular_app)

