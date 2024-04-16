#include "pico/stdlib.h"
#include "max31855.h"

static inline void max31855_cs_select(uint pin_cs) {
    asm volatile("nop \n nop \n nop");
    gpio_put(pin_cs, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void max31855_cs_deselect(uint pin_cs) {
    asm volatile("nop \n nop \n nop");
    gpio_put(pin_cs, 1);
    asm volatile("nop \n nop \n nop");
}

void max31855_init(const max31855_config *config) {
    spi_init(config->spi, 5000 * 1000); // 5 MHz
    gpio_set_function(config->pin_data, GPIO_FUNC_SPI);
    gpio_set_function(config->pin_sck, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(config->pin_cs);
    gpio_set_dir(config->pin_cs, GPIO_OUT);
    gpio_put(config->pin_cs, 1);
}

void max31855_read(const max31855_config *config, struct max31855_result *result) {
    uint8_t buffer[4];
    
    max31855_cs_select(config->pin_cs);
    sleep_ms(1);
    spi_read_blocking(config->spi, 0, buffer, 4);
    max31855_cs_deselect(config->pin_cs);
    sleep_ms(1);

    int16_t thermocouple = MAX31855_SIGN_EXTEND((buffer[0] << 6) | (buffer[1] >> 2), 14);
    int16_t internal = MAX31855_SIGN_EXTEND((buffer[2] << 4) | (buffer[3] >> 4), 12);

    result->thermocouple = (1.0f * thermocouple) / 4;
    result->internal = (1.0f * internal) / 16;
    result->faults = ((buffer[1] & 0x01) << 8) | (buffer[3] & 0x07);
}