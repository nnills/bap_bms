// The header file for BMS functions and structs

#include "stdint.h"

struct slaveReadout
{
    uint16_t cells[18] = {0};
    uint16_t temperature[16] = {0};
    uint16_t current = 0;
    int16_t current_ma = 0;
};

