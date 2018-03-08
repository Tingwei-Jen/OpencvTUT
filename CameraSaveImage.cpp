#include <opencv2/opencv.hpp>
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

using namespace cv;
using namespace std;

int main(int argc, char** argv ){


    //VideoCapture cap(" tcambin ! videoconvert ! appsink"); // open the default camera
    VideoCapture cap(1);

    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    Mat frame;
    namedWindow("Webcamera",1);
    
    int i=100;
    while(1)
    {
        cap.read(frame); // get a new frame from camera
        imshow("Webcamera", frame);
        if(waitKey(30) >= 0) {

            string filename;
            filename = "fish" + std::to_string(i)+ ".jpg";
            imwrite (filename, frame);
            i++;
            cout<<i<<endl;
        }

    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
