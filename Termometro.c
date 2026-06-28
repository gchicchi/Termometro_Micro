#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

#include "pico-ssd1306-main/ssd1306.h"

// -------------------- PINOS --------------------

#define ADC_PIN     26
#define BUZZER_PIN  15

#define SDA_PIN     0
#define SCL_PIN     1

// -------------------- OLED ---------------------

ssd1306_t ssd;

// -------------------- BUZZER --------------------

void beep(uint duration_ms)
{
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);

    pwm_set_clkdiv(slice, 125.0f);
    pwm_set_wrap(slice, 1000);

    pwm_set_gpio_level(BUZZER_PIN, 500);

    pwm_set_enabled(slice, true);

    sleep_ms(duration_ms);

    pwm_set_enabled(slice, false);
}

// ---------------- TEMPERATURA ------------------

float adc_to_voltage(uint16_t adc_value)
{
    return ((float)adc_value * 3.3f) / 4095.0f;
}

float voltage_to_temperature(float voltage)
{
    return logf(voltage / 0.3092f) / 0.0442f;
}

// Média de 50 amostras para reduzir ruído

uint16_t adc_media()
{
    uint32_t soma = 0;

    for(int i = 0; i < 50; i++)
    {
        soma += adc_read();
        sleep_us(100);
    }

    return soma / 50;
}

// -------------------- MAIN ---------------------

int main()
{
    stdio_init_all();

    // ADC
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);

    // I2C OLED
    i2c_init(i2c0, 400000);

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    ssd1306_init(&ssd, 128, 64, 0x3C, i2c0);

    // Buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    // Bip ao ligar o Pico
    beep(200);
    sleep_ms(100);

    float temp_anterior = -999.0f;

    float temperatura_filtrada = 0.0f;
    bool primeira_leitura = true;

    int contador_estavel = 0;
    bool ja_apitou = false;

    bool alerta_febre = false;

    while (true)
    {
        uint16_t adc_raw = adc_media();

        float tensao = adc_to_voltage(adc_raw);

        float temperatura =
            voltage_to_temperature(tensao);

        // Filtro exponencial

        if(primeira_leitura)
        {
            temperatura_filtrada = temperatura;
            primeira_leitura = false;
        }
        else
        {
            temperatura_filtrada =
                0.9f * temperatura_filtrada +
                0.1f * temperatura;
        }
     

        //-------------------------
        // OLED
        //-------------------------

        char linha1[32];
        char linha2[32];

        if(temperatura_filtrada > 38.0f)
        {
            sprintf(linha1, "FEBRE!");
            sprintf(linha2, "%.2f C", temperatura_filtrada);
        }
        else
        {
            sprintf(linha1, "Temp:");
            sprintf(linha2, "%.2f C", temperatura_filtrada);
        }

        ssd1306_clear(&ssd);

        ssd1306_draw_string(&ssd, 0, 0, 2, linha1);
        ssd1306_draw_string(&ssd, 0, 25, 2, linha2);

        ssd1306_show(&ssd);

        // Alerta de febre
        if(temperatura_filtrada > 38.0f)
        {
            if(!alerta_febre)
            {
                beep(200);
                alerta_febre = true;
            }
        }
        else
        {
            alerta_febre = false;
        }
        //-------------------------
        // ESTABILIDADE
        //-------------------------

        if(fabs(temperatura_filtrada - temp_anterior) < 0.2f)
        {
            contador_estavel++;
        }
        else
        {
            contador_estavel = 0;
            ja_apitou = false;
        }

        temp_anterior = temperatura_filtrada;

        if(contador_estavel >= 30 && !ja_apitou)
        {
            beep(500);
            ja_apitou = true;
        }

        sleep_ms(1000);
    }
}