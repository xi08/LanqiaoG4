#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"

uint8_t DHT11_ReadData(float *humi, float *temp);

#ifdef __cplusplus
}
#endif

#endif // !__DHT11_H
