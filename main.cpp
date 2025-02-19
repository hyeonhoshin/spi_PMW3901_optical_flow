/*
 * Example user-space driver for the PMW3901 Optical Flow sensor 
 * from PixArt.  SPI interface.
 *
 * Meant for use with NVidia Jetson TX2, but should probably work with others.
 *
 * Copyright 2018 Mark Fassler (driver code originally from Bitcraze AB).
 *
 */
extern "C" {
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "Bitcraze_PMW3901.h"
}

#include "cv.hpp"

static const char *device = "/dev/spidev0.0";
static uint8_t mode = 3;//Basic mode setting 0. 3 is properly worked;
static uint32_t speed = 5000000;

static char Fbuffer[35*35] = {0x08};


int main(int argc, char *argv[]) {

	int ret;
	int fd;

	int16_t deltaX;
	int16_t deltaY;

	printf("Hello there.\n");

	test_cv("mary.jpg");

	// Open SPI channel
	fd = open(device, O_RDWR);
	if (fd < 0) {
		perror("can't open device");
		exit(1);
	}


	// set SPI mode
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret < 0) {
		perror("can't set SPI mode");
		exit(1);
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret < 0) {
		perror("can't read SPI mode");
		exit(1);
	}


	// max speed Hz
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret < 0) {
		perror("can't set max speed hz");
		exit(1);
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret < 0) {
		perror("can't get max speed hz");
		exit(1);
	}

	/*
	Bitcraze_PMW3901_init(fd);
	enableFrameBuffer(fd);

	while (1) {
		usleep(33000);

		//Bitcraze_PMW3901_readMotionCount(fd, &deltaX, &deltaY);

		//printf("%d %d\n", deltaX, deltaY);
		readFrameBuffer(fd, Fbuffer);

		int seeing = 0;
		char prior = 0x08;
		for(int i = 0; i<35; i++){
			printf("Line %d : ",i);
			for(int j=0; j<35; j++){
				seeing++;
				printf("%03d ",Fbuffer[seeing]);

				if(Fbuffer[seeing]==0 & prior == 0){
					printf("The 0s occur from row %d, col %d\n",i,j);
					return -1;
				}
				prior = Fbuffer[seeing];
			}
			printf("\n");
		}
		printf("\n");

	}
	*/

	return 0;
}

