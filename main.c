
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "Bitcraze_PMW3901.h"


static const char *device = "/dev/spidev3.0";
static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay = 1;


int main(int argc, char *argv[]) {

	int ret;
	int fd;

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

	return 0;
}

