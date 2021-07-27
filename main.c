/*
 * Example user-space driver for the PMW3901 Optical Flow sensor 
 * from PixArt.  SPI interface.
 *
 * Meant for use with NVidia Jetson TX2, but should probably work with others.
 *
 * Copyright 2018 Mark Fassler (driver code originally from Bitcraze AB).
 *
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "Bitcraze_PMW3901.h"

#define FRAME_NUM 2

#define __DEBUG__

static const char *device = "/dev/spidev0.0";
static uint8_t mode = 3;//Basic mode setting 0. 3 is properly worked;
static uint32_t speed = 20000000; //20Mhz.

static char Fbuffer[FRAME_NUM][35*35] = {0x08};


int main(int argc, char *argv[]) {

	if(argc < 2){
		printf("Error. Few elements.\n");
		return -1;
	}

	int ret;
	int fd;

	int16_t deltaX;
	int16_t deltaY;

	printf("Hello there.\n");

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

	Bitcraze_PMW3901_init(fd);
	printf("Sensor initialized...\n");

	enableFrameBuffer(fd);
	printf("Frame buffer initialized...\n");

	usleep(33000);

	printf("Before read : \n");

	for(int frame = FRAME_NUM-1; frame>=0; frame--){
		for(int i = 0; i<35; i++){
			for(int j = 0; j<35; j++){
				printf("%d ",Fbuffer[frame][35*i+j]);
			}
			printf("\n");
		}
	}
	
	readFrameBuffer(fd, Fbuffer[0]);
	readFrameBuffer(fd, Fbuffer[1]);

	printf("After read : \n");

	for(int frame = FRAME_NUM-1; frame>=0; frame--){
		for(int i = 0; i<35; i++){
			for(int j = 0; j<35; j++){
				printf("%d ",Fbuffer[frame][35*i+j]);
			}
			printf("\n");
		}
	}

	printf("Writing into... %s\n",argv[1]);

	FILE* fp_txt = fopen(argv[1], "w");

	for(int frame = FRAME_NUM-1; frame>=0; frame--){
		#ifdef __DEBUG__
		fprintf(fp_txt,"Frame %d : \n",frame);
		#endif
		for (int i = 0; i < 35; i++) {
			for (int j = 1; j < 35; j++) {
				if (j != 34) fprintf(fp_txt, "%d,", Fbuffer[frame][35*i+j]);
				else fprintf(fp_txt, "%d\n", Fbuffer[frame][35*i+j]);
			}
		}
	}

	fclose(fp_txt);
	
	return 0;
}

