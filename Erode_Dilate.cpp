#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main(int argc, char** argv )
{
    if ( argc != 2 ){
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image, imagetres;
    image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    threshold(image,imagetres,120,255,THRESH_BINARY);

    if ( !image.data ){
        printf("No image data \n");
        return -1;
    }
    int count = 0;
    for(int i=0;i<480;i++){
    	for(int j=0;j<744;j++){
    		int value1 = image.at<char>(i,j); 
    		if(value1>100){
    			count++;
    			cout<<"( "<<i<<","<<j<<")  "<<"value1: "<<value1<<endl;    		
    			cout<<"count:"<<count;
    		}
    	}
    }
    


    Mat dst;
    Mat dilateStruct = getStructuringElement(MORPH_RECT,Size(10,10));
    dilate(imagetres, dst, dilateStruct);
    
    //Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(5,5));
    //erode(image2, dst3, erodeStruct);

    //namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Led", image);
    //imshow("Dilate", dst);


    waitKey(0);
    return 0;
}
