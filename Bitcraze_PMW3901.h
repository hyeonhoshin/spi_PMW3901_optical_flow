

#ifndef __SPI_PMW3901
#define __SPI_PMW3901

extern int Bitcraze_PMW3901_init(int);
extern void Bitcraze_PMW3901_readMotionCount(int, int16_t*, int16_t*);

extern void enableFrameBuffer(int fd);
extern void readFrameBuffer(int fd, char *FBuffer);

#endif // __SPI_PMW3901


