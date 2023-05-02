#ifndef __SWDELAY_H

#include "main.h"

#define delay_us(x) swDelay5((x)*80 / 5)
#define delay_1WireFast(x) swDelay5((x)*80 / 20)

void swDelay5(uint32_t n);

#endif // !__SWDELAY_H
