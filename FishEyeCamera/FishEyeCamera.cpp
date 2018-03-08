#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

double getMilliSeconds()
{
    chrono::duration<double, std::nano> ns = chrono::duration_cast< chrono::duration<double, nano> >(chrono::system_clock::now().time_since_epoch());
    return ns.count() * 1.0e-6;
}

VideoCapture getVideo(int id, int width, int height)
{
    // Start default camera
    VideoCapture video(id);
    video.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    video.set(CV_CAP_PROP_FRAME_WIDTH, width);
    video.set(CV_CAP_PROP_FRAME_HEIGHT, height);

    return video;
}

double getFps(VideoCapture video)
{   
    // With webcam get(CV_CAP_PROP_FPS) does not work.
    // Let's see for ourselves.
    // If you do not care about backward compatibility
    // You can use the following instead for OpenCV 3
    // double fps = video.get(CAP_PROP_FPS);
    return  video.get(CV_CAP_PROP_FPS);
}

double CalculateFps(VideoCapture video, int num_frames)
{
    Mat frame; 
    cout << "Give up beginning some frames " << endl ;
    for(int i = 0; i < 30; i++)
    {
        video.read(frame);
    }

    cout << "Grab some frames" << endl ;
    double t = getMilliSeconds();
    for(int i = 0; i < num_frames; i++)
    {
        video.read(frame);
    }
    double t1 = getMilliSeconds();  

    double seconds = (t1-t)/1000;
    cout << "Time taken : " << seconds << " seconds" << endl;
     
    // Calculate frames per second
    return (num_frames / seconds);
}

int main(){

    VideoCapture video = getVideo(0, 640, 480);

    double fps = getFps(video);
    cout << "Frames per second using video.get(CV_CAP_PROP_FPS) : " << fps << endl;
     
    // Number of frames to capture
    int num_frames = 500;
    cout << "Capturing " << num_frames << " frames" << endl ;

    fps = CalculateFps(video, num_frames);
    cout << "Estimated frames per second : " << fps << endl;

    Mat frame;
    int i=100;
    while(1)
    {
        video.read(frame); 
        imshow("camera", frame);
        
        string filename;
        filename = std::to_string(i)+ ".jpg";
        imwrite (filename, frame);
        i++;
        cout<<i<<endl;
        
        if(waitKey(30) >= 0) 
            break;

    }
    // Release video
    video.release();
    return 0;
}