#include "cv.hpp"

using namespace cv;
int test_cv(std::string img_name)
{
    Mat image;
    image = imread( img_name, 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
    return 0;
}