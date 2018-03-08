#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <chrono>

using namespace std;
using namespace cv;

double getMilliSeconds()
{
    chrono::duration<double, std::nano> ns = chrono::duration_cast< chrono::duration<double, nano> >(chrono::system_clock::now().time_since_epoch());
    return ns.count() * 1.0e-6;
}

bool ORB_Video();

int main ( int argc, char** argv )
{
   
    if ( argc != 3 )
    {
        cout<<"usage: feature_extraction img1 img2"<<endl;
        return 1;
    }
    //-- 读取图像
    Mat img_1_ORIGIN = imread ( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    Mat img_2_ORIGIN = imread ( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

    Mat img_1 =img_1_ORIGIN-Scalar(75,75,75);
    Mat img_2 =img_2_ORIGIN-Scalar(75,75,75);

    imshow("img",img_1);
    //-- 初始化
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    Ptr<FeatureDetector> detector = ORB::create(100, 1.2f, 8, 31);
    Ptr<DescriptorExtractor> descriptor = ORB::create();
    // Ptr<FeatureDetector> detector = FeatureDetector::create(detector_name);
    // Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create(descriptor_name);
    Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );

    //-- 第一步:检测 Oriented FAST 角点位置
    detector->detect ( img_1,keypoints_1 );
    detector->detect ( img_2,keypoints_2 );

    //-- 第二步:根据角点位置计算 BRIEF 描述子
    descriptor->compute ( img_1, keypoints_1, descriptors_1 );
    descriptor->compute ( img_2, keypoints_2, descriptors_2 );

    Mat outimg1;
    drawKeypoints( img_1, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    imshow("ORB特征点",outimg1);

    //-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
    vector<DMatch> matches;
    //BFMatcher matcher ( NORM_HAMMING );
    matcher->match ( descriptors_1, descriptors_2, matches );

    //-- 第四步:匹配点对筛选
    double min_dist=10000, max_dist=0;

    //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if ( dist < min_dist ) min_dist = dist;
        if ( dist > max_dist ) max_dist = dist;
    }
    
    // 仅供娱乐的写法
    min_dist = min_element( matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance<m2.distance;} )->distance;
    max_dist = max_element( matches.begin(), matches.end(), [](const DMatch& m1, const DMatch& m2) {return m1.distance<m2.distance;} )->distance;

    printf ( "-- Max dist : %f \n", max_dist );
    printf ( "-- Min dist : %f \n", min_dist );

    //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.
    std::vector< DMatch > good_matches;
    for ( int i = 0; i < descriptors_1.rows; i++ )
    {
        if ( matches[i].distance <= max ( 1.0*min_dist, 30.0 ) )
        {
            good_matches.push_back ( matches[i] );
        }
    }

    //-- 第五步:绘制匹配结果
    Mat img_match;
    Mat img_goodmatch;
    drawMatches ( img_1, keypoints_1, img_2, keypoints_2, matches, img_match );
    drawMatches ( img_1, keypoints_1, img_2, keypoints_2, good_matches, img_goodmatch );
    imshow ( "所有匹配点对", img_match );
    imshow ( "优化后匹配点对", img_goodmatch );
    waitKey(0);
    return 0;
}

bool ORB_Video()
{

    //Initial
    std::vector<KeyPoint> keypoints;
    Ptr<FeatureDetector> detector = ORB::create();

    //open camera
    VideoCapture cap(1);
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    Mat frame;
    Mat outimg;
    double period;

    while(1){

        double t0 = getMilliSeconds();
        cap.read(frame);                          // get a new frame from camera
        cv::cvtColor(frame, frame, CV_RGB2GRAY);  //turn to gray
        detector->detect ( frame,keypoints );
        double t1 = getMilliSeconds();

        period = t1-t0;
        cout<<"period: "<<period<<endl;

        drawKeypoints( frame, keypoints, outimg, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
        imshow("ORB特征点",outimg);
        //imshow("USBcamera", frame);
        if(waitKey(30) >= 0) break;
    }
    return true;
}