#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
int main(int argc, char** argv )
{
    if ( argc != 2 ){
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image, image2;
    image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    threshold(image,image2,120,255,THRESH_BINARY);

    if ( !image.data ){
        printf("No image data \n");
        return -1;
    }

    Mat dst1,dst2,dst3;
    erode(image2, dst1, Mat());
    dilate(image2, dst2, Mat());
    
    Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(5,5));
    erode(image2, dst3, erodeStruct);



    namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    imshow("Threshold", image2);

    imshow("Erode", dst1);
    imshow("Dilate", dst2);
    imshow("Erode2", dst3);
    waitKey(0);
    return 0;
}
