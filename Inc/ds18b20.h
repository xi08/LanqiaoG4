#ifndef __DS18B20_H
#define __DS18B20_H

#include "main.h"

uint8_t DS18B20_Start(uint8_t Res);
uint8_t DS18B20_Conv(float *temp);

#endif //  __DS18B20_H
