extern "C"{
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
}

#include <string>
#include <opencv2/opencv.hpp>

int test_cv(std::string img_name);