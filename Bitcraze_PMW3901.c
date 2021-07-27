/* PMW3901 Arduino driver
 * Copyright (c) 2017 Bitcraze AB
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


// Code was originally from:  https://github.com/bitcraze/Bitcraze_PMW3901.git
// More changes, Copyright 2018 Mark Fassler


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>


static uint8_t bits = 8;
static uint32_t speed = 20000000; //20Mhz.
static uint16_t delay = 1;


static uint8_t registerRead(int fd, uint8_t reg) {
	uint8_t tx_buffer[2];
	uint8_t rx_buffer[2];
	int ret;

	tx_buffer[0] = reg;
	tx_buffer[1] = 0;

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buffer,
		.rx_buf = (unsigned long)rx_buffer,
		.len = 2,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		perror("can't send spi message");
		exit(1);
	}

	return rx_buffer[1];
}



static int registerWrite(int fd, uint8_t reg, uint8_t value) {

	int ret;
	uint8_t tx_buffer[2];

	tx_buffer[0] = reg | 0x80;  // msb sets a "write" operation, I presume?
	tx_buffer[1] = value;

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx_buffer,
		.rx_buf = 0,
		.len = 2,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1) {
		perror("can't send spi message");
		exit(1);
	}

	return 0;
}



// Performance optimisation registers
static void initRegisters(int fd) {

	registerWrite(fd, 0x7F, 0x00);
	registerWrite(fd, 0x61, 0xAD);
	registerWrite(fd, 0x7F, 0x03);
	registerWrite(fd, 0x40, 0x00);
	registerWrite(fd, 0x7F, 0x05);
	registerWrite(fd, 0x41, 0xB3);
	registerWrite(fd, 0x43, 0xF1);
	registerWrite(fd, 0x45, 0x14);
	registerWrite(fd, 0x5B, 0x32);
	registerWrite(fd, 0x5F, 0x34);
	registerWrite(fd, 0x7B, 0x08);
	registerWrite(fd, 0x7F, 0x06);
	registerWrite(fd, 0x44, 0x1B);
	registerWrite(fd, 0x40, 0xBF);
	registerWrite(fd, 0x4E, 0x3F);
	//  ... one of the datasheets ends here.  But the code from BitCraze keeps going below:

	registerWrite(fd, 0x7F, 0x08);
	registerWrite(fd, 0x65, 0x20);
	registerWrite(fd, 0x6A, 0x18);
	registerWrite(fd, 0x7F, 0x09);
	registerWrite(fd, 0x4F, 0xAF);
	registerWrite(fd, 0x5F, 0x40);
	registerWrite(fd, 0x48, 0x80);
	registerWrite(fd, 0x49, 0x80);
	registerWrite(fd, 0x57, 0x77);
	registerWrite(fd, 0x60, 0x78);
	registerWrite(fd, 0x61, 0x78);
	registerWrite(fd, 0x62, 0x08);
	registerWrite(fd, 0x63, 0x50);
	registerWrite(fd, 0x7F, 0x0A);
	registerWrite(fd, 0x45, 0x60);
	registerWrite(fd, 0x7F, 0x00);
	registerWrite(fd, 0x4D, 0x11);
	registerWrite(fd, 0x55, 0x80);
	registerWrite(fd, 0x74, 0x1F);
	registerWrite(fd, 0x75, 0x1F);
	registerWrite(fd, 0x4A, 0x78);
	registerWrite(fd, 0x4B, 0x78);
	registerWrite(fd, 0x44, 0x08);
	registerWrite(fd, 0x45, 0x50);
	registerWrite(fd, 0x64, 0xFF);
	registerWrite(fd, 0x65, 0x1F);
	registerWrite(fd, 0x7F, 0x14);
	registerWrite(fd, 0x65, 0x60);
	registerWrite(fd, 0x66, 0x08);
	registerWrite(fd, 0x63, 0x78);
	registerWrite(fd, 0x7F, 0x15);
	registerWrite(fd, 0x48, 0x58);
	registerWrite(fd, 0x7F, 0x07);
	registerWrite(fd, 0x41, 0x0D);
	registerWrite(fd, 0x43, 0x14);
	registerWrite(fd, 0x4B, 0x0E);
	registerWrite(fd, 0x45, 0x0F);
	registerWrite(fd, 0x44, 0x42);
	registerWrite(fd, 0x4C, 0x80);
	registerWrite(fd, 0x7F, 0x10);
	registerWrite(fd, 0x5B, 0x02);
	registerWrite(fd, 0x7F, 0x07);
	registerWrite(fd, 0x40, 0x41);
	registerWrite(fd, 0x70, 0x00);

	usleep(100000);
	registerWrite(fd, 0x32, 0x44);
	registerWrite(fd, 0x7F, 0x07);
	registerWrite(fd, 0x40, 0x40);
	registerWrite(fd, 0x7F, 0x06);
	registerWrite(fd, 0x62, 0xf0);
	registerWrite(fd, 0x63, 0x00);
	registerWrite(fd, 0x7F, 0x0D);
	registerWrite(fd, 0x48, 0xC0);
	registerWrite(fd, 0x6F, 0xd5);
	registerWrite(fd, 0x7F, 0x00);
	registerWrite(fd, 0x5B, 0xa0);
	registerWrite(fd, 0x4E, 0xA8);
	registerWrite(fd, 0x5A, 0x50);
	registerWrite(fd, 0x40, 0x80);
}



int Bitcraze_PMW3901_init(int fd) {
	// Power on reset
	registerWrite(fd, 0x3A, 0x5A);
	usleep(5000);

	// Test the SPI communication, checking chipId and inverse chipId
	uint8_t chipId = registerRead(fd, 0x00);
	uint8_t dIpihc = registerRead(fd, 0x5F);

	printf("chipId: %x  dIpihc: %x \n", chipId, dIpihc);

	if (chipId != 0x49 && dIpihc != 0xB8) {
		printf("bad chipId\n");
		return -1;
	}

	printf("YAY!  Good chipId\n");

	// Reading the motion registers one time
	registerRead(fd, 0x02);
	registerRead(fd, 0x03);
	registerRead(fd, 0x04);
	registerRead(fd, 0x05);
	registerRead(fd, 0x06);
	usleep(1000);

	initRegisters(fd);

	return 0;
}



void Bitcraze_PMW3901_readMotionCount(int fd, int16_t *deltaX, int16_t *deltaY) {

	registerRead(fd, 0x02);

	*deltaX = ((int16_t)registerRead(fd, 0x04) << 8) | registerRead(fd, 0x03);
	*deltaY = ((int16_t)registerRead(fd, 0x06) << 8) | registerRead(fd, 0x05);
}

// Got from Arduino version PMW3901
void enableFrameBuffer(int fd)
{

  registerWrite(fd, 0x7F, 0x07);  //Magic frame readout registers
  registerWrite(fd, 0x41, 0x1D);
  registerWrite(fd, 0x4C, 0x00);
  registerWrite(fd, 0x7F, 0x08);
  registerWrite(fd, 0x6A, 0x38);
  registerWrite(fd, 0x7F, 0x00);
  registerWrite(fd, 0x55, 0x04);
  registerWrite(fd, 0x40, 0x80);
  registerWrite(fd, 0x4D, 0x11);

  registerWrite(fd, 0x70, 0x00);   //More magic? 
  registerWrite(fd, 0x58, 0xFF);

  int temp = registerRead(fd, 0x58); //Read status register 
  int check = temp>>6; //rightshift 6 bits so only top two stay

  while(check == 0x03){ //while bits aren't set denoting ready state
    temp = registerRead(fd, 0x58); //keep reading and testing 
    check = temp>>6; //shift again 
  }  
  usleep(50);
}

void readFrameBuffer(int fd, char *FBuffer)
{
  int count = 0;
  uint8_t a; //temp value for reading register
  uint8_t b; //temp value for second register
  uint8_t hold; //holding value for checking bits
  uint8_t mask = 0x0c; //mask to take bits 2 and 3 from b
  uint8_t pixel = 0; //temp holding value for pixel

  for (int ii = 0; ii < 1225; ii++) { //for 1 frame of 1225 pixels (35*35)
    //check status bits 6 and 7
    //if 01 move upper 6 bits into temp value
    //if 00 or 11, reread
    //else lower 2 bits into temp value
    a = registerRead(fd, 0x58); //read register
    hold = a >> 6; //right shift to leave top two bits for ease of check.

    while ((hold == 0x03) || (hold == 0x00)) { //if data is either invalid status
      a = registerRead(fd, 0x58); //reread loop
      hold = a >> 6;
    }
    if (hold == 0x01) { //if data is upper 6 bits
      b = registerRead(fd, 0x58); //read next set to get lower 2 bits
      pixel = a; //set pixel to a
      pixel = pixel << 2; //push left to 7:2
      pixel += (b & mask); //set lower 2 from b to 1:0
      FBuffer[count++] = pixel; //put temp value in fbuffer array
	  printf("p : %d", pixel);
      //delayMicroseconds(100);
    }
    else {}
  }
  registerWrite(fd, 0x70, 0x00);   //More magic? 
  registerWrite(fd, 0x58, 0xFF);

  int temp = registerRead(fd, 0x58); //Read status register 
  int check = temp>>6; //rightshift 6 bits so only top two stay

  while(check == 0x03){ //while bits aren't set denoting ready state
    temp = registerRead(fd, 0x58); //keep reading and testing 
    check = temp>>6; //shift again 
  }  
}