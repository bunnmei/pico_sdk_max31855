#include <stdio.h>
#include <stdlib.h>
#include "max31855.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define MISO 16 //(DO) 
#define CS   17
#define SCLK 18

#define MISO2 4
#define CS2 5
#define SCLK2 2 
// #define MOSI 19

#define SPI_PORT spi0

int main() {
    stdio_init_all();

    max31855_config conf = {
        SPI_PORT,
        MISO,
        CS,
        SCLK
    };

    max31855_config conf2 = {
        SPI_PORT,
        MISO2,
        CS2,
        SCLK2
    };

    max31855_init(&conf);
    max31855_init(&conf2);

    struct max31855_result result;
    struct max31855_result result2;
    while (true)
    {
        max31855_read(&conf, &result);
        max31855_read(&conf2, &result2);
        printf("1-thermocouple -- %f \n", result.thermocouple);
        printf("2-thermocouple -- %f \n", result2.thermocouple);
        sleep_ms(1000);
    }
}

// https://gist.github.com/ludekstepan/1463643d743db24fdf051b121b5b8968