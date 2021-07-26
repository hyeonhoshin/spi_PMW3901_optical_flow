

#ifndef __SPI_PMW3901
#define __SPI_PMW3901

int Bitcraze_PMW3901_init(int);
void Bitcraze_PMW3901_readMotionCount(int, int16_t*, int16_t*);

void enableFrameBuffer(int);
void readFrameBuffer(int, char*);

#endif // __SPI_PMW3901


