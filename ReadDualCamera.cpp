#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv){

	VideoCapture cap;
    cap.open(1);
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);

    Mat frame;
    namedWindow("WebCamera",1);
    
    Mat left;
    Mat right;

    while(1){
        cap.read(frame); // get a new frame from camera
        cout<<frame.rows<<"  "<<frame.cols<<endl;
        imshow("WebCamera", frame);
        if(waitKey(30) >= 0) break;
    }
	return 0;
}