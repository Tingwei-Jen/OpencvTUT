#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

int main(){

   
	VideoCapture cap(1);
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    namedWindow("WebCamera",1);
    
    while(1){
        cap.read(frame); // get a new frame from camera
        std::cout<<frame.rows<<"  "<<frame.cols<<std::endl;
        imshow("WebCamera", frame);
        if(waitKey(30) >= 0) break;
    }
	return 0;
}