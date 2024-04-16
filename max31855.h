#ifndef _MAX31855_H
#define _MAX31855_H

#include <stdint.h>
#include "hardware/spi.h"

#define MAX31855_FAULT     0x100    // 1 when any of the SCV, SCG, or OC faults are active
#define MAX31855_SCV_FAULT  0x04    // short-circuited to VCC
#define MAX31855_SCG_FAULT  0x02    // short-circuited to GND
#define MAX31855_OC_FAULT   0x01    // open (no connections)

#define MAX31855_SIGN_EXTEND(value, bit) (((value) ^ (1u << ((bit) - 1))) - (1u << ((bit) - 1)))

typedef struct {
    spi_inst_t *spi;

    uint pin_data;
    uint pin_cs;
    uint pin_sck;
} max31855_config;

struct max31855_result
{
    float thermocouple;
    float internal;
    uint16_t faults;
};

void max31855_init(const max31855_config *config);

void max31855_read(const max31855_config *config, struct max31855_result *result);

#endif