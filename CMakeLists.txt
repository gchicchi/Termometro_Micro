cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(sensor_temperatura C CXX ASM)

pico_sdk_init()

add_executable(sensor_temperatura
    Termometro.c
    pico-ssd1306-main/ssd1306.c
)

target_link_libraries(sensor_temperatura
    pico_stdlib
    hardware_adc
    hardware_i2c
    hardware_pwm
)

# ATENÇÃO: Habilita suporte completo a float/double no printf e sprintf
pico_enable_stdio_uart(sensor_temperatura 0)
pico_enable_stdio_usb(sensor_temperatura 1)

pico_add_extra_outputs(sensor_temperatura)