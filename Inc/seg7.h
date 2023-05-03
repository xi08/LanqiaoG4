#ifndef __SEG_H

#include "main.h"

extern const uint8_t seg7Mask[20];

void segInit(void);

void segSend(uint8_t pos1, uint8_t pos2, uint8_t pos3);

#endif // !__SEG_H
