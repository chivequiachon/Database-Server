#include "checksum.h"

unsigned char Fletcher::calculateChecksum(unsigned char *packet, int len)
{
    unsigned short sum1 = 0xf, sum2 = 0xf;
    
	while(len)
	{
        sum1 += *packet++;
        sum2 += sum1;
        len--;
    };
    
	sum1 = (sum1 & 0x0f) + (sum1 >> 4);
    sum1 = (sum1 & 0x0f) + (sum1 >> 4);
    sum2 = (sum2 & 0x0f) + (sum2 >> 4);
    sum2 = (sum2 & 0x0f) + (sum2 >> 4);
    
	return sum2 << 4 | sum1;
}