#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>

using namespace cv;
using namespace std;

double getMilliSeconds(){
    chrono::duration<double, std::nano> ns = chrono::duration_cast< chrono::duration<double, nano> >(chrono::system_clock::now().time_since_epoch());
    return ns.count() * 1.0e-6;
}

int main(int argc, char** argv ){

    //gst-launch-1.0 v4l2src ! videoconvert ! autovideosink   //開筆電攝影頭
    //gst-launch-1.0 tcambin ! videoconvert ! autovideosink   //open DMK camera
    VideoCapture cap(" tcambin ! videoconvert ! appsink"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    // Number of frames to capture
    int num_frames = 120;
    // Variable for storing video frames
    Mat srcframe;
    Mat dstframe;

/*    cout << "Capturing " << num_frames << " frames" << endl ;
    double duration, start, finish;

    // Start time
    start = getMilliSeconds();
     
    // Grab a few frames
    for(int i = 0; i < num_frames; i++)
    {
        cap >> srcframe;
        cv::resize(srcframe,dstframe,cv::Size(640,480));
    }
    // End Time
    finish = getMilliSeconds();

    duration = finish - start;
    printf( "Time taken: %f milliSeconds\n", duration);     
    printf( "%f FPS\n", num_frames/(duration/1000));

    cap.release();

    cv::Size s = dstframe.size();
    cout<<"Height of Frame: "<<s.height<<endl;
    cout<<"Width of Frame: "<<s.width<<endl;*/

    namedWindow("DMK",1);
    for(;;)
    {
        cap >> srcframe; // get a new frame from camera
        cv::resize(srcframe,dstframe,cv::Size(640,480));
        imshow("DMK", dstframe);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
