#include "stdafx.h"
#include "controllerdefs.h"


uint8_t checksum(uint8_t *commands, int length)
{
    uint8_t checksum = 0;
    int i;
    for(i=0; i < length; i++) {
        checksum += ((*commands++) & 0xFF);
    }
    return checksum;
}


