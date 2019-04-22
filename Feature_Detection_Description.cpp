#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

/* @function readme */
void readme(){ 
	std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; 
}

void DetectFeatureFast(Mat img)
{
    Ptr<FastFeatureDetector> fastDetector = FastFeatureDetector::create();
    vector<KeyPoint> keyPoints;
    fastDetector->detect(img, keyPoints);
    Mat img_keypoints;
    drawKeypoints( img, keyPoints, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    imshow("img_keypoints", img_keypoints);
    waitKey(0);
}

void DetectFeatureSURF(Mat img, int minHessian){

  	//-- Step 1: Detect the keypoints using SURF Detector
  	Ptr<SURF> detector = SURF::create( minHessian );
  	std::vector<KeyPoint> keypoints;
  	detector->detect( img, keypoints );
  	//-- Step 2: Draw keypoints
  	Mat img_keypoints;
  	drawKeypoints( img, keypoints, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  	//-- Step 3: Show detected (drawn) keypoints
  	imshow("KeypointsSURF", img_keypoints );
}

void DetectFeatureSIFT(Mat img){

	//-- Step 1: Detect the keypoints using SIFT Detector
    Ptr<SIFT> detector = SIFT::create(17000);
    std::vector<KeyPoint> keypoints;
    detector->detect( img, keypoints );
    //-- Step 2: Draw keypoints
    Mat img_keypoints;
    drawKeypoints( img, keypoints, img_keypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    //-- Step 3: Show detected (drawn) keypoints
    imshow("KeypointsSIFT", img_keypoints );
}

void FeatureDescription(Mat img_1, Mat img_2){


  //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
  int minHessian = 400;
  Ptr<SURF> detector = SURF::create();
  detector->setHessianThreshold(minHessian);
  std::vector<KeyPoint> keypoints_1, keypoints_2;
  Mat descriptors_1, descriptors_2;
  detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
  detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );
  //-- Step 2: Matching descriptor vectors with a brute force matcher
  BFMatcher matcher(NORM_L2);
  std::vector< DMatch > matches;
  matcher.match( descriptors_1, descriptors_2, matches );
  //-- Draw matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_matches );
  //-- Show detected matches
  imshow("Matches", img_matches );
}

void FeatureMatching_FLANN(Mat query_img, Mat train_img){

	//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
	int minHessian = 400;
  	Ptr<SURF> detector = SURF::create();
  	detector->setHessianThreshold(minHessian);
	std::vector<KeyPoint> keypoints_query, keypoints_train;
  	Mat descriptors_query, descriptors_train;
  	detector->detectAndCompute( query_img, Mat(), keypoints_query, descriptors_query );
  	detector->detectAndCompute( train_img, Mat(), keypoints_train, descriptors_train );

  	//-- Step 2: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_query, descriptors_train, matches );

    double max_dist = 0; double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    //两个特征向量之间的欧氏距离，越小表明匹配度越高。
    //0ρ = √( (x1-x2)2+(y1-y2)2 )　|x| = √( x2 + y2 )

    for( int i = 0; i < descriptors_query.rows; i++ ){ 
    	double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );

    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
    //-- small)
    //-- PS.- radiusMatch can also be used here.
    std::vector< DMatch > good_matches;

  	for( int i = 0; i < descriptors_query.rows; i++ ){
  		if( matches[i].distance <= max(2*min_dist, 0.02) ){ 
  			good_matches.push_back( matches[i]); 
  		}
  	}

  	//-- Draw only "good" matches
  	Mat img_matches;
  	drawMatches( query_img, keypoints_query, train_img, keypoints_train,
                 good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                 vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //-- Show detected matches
    imshow( "Good Matches", img_matches );

    //Keypoint index
    for( int i = 0; i < (int)good_matches.size(); i++ ){ 
  	  printf( "-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx ); 
    }

    //-- Localize
    std::vector<Point2f> query;
    std::vector<Point2f> train;
    for( size_t i = 0; i < good_matches.size(); i++ ){
    	//-- Get the keypoints from the good matches
    	Point2f qp = keypoints_query[ good_matches[i].queryIdx ].pt;
    	Point2f tp = keypoints_train[ good_matches[i].trainIdx ].pt;
    	query.push_back( qp );
    	train.push_back( tp );
    	printf( "-- Good Match [%d] -- Keypoint query: x: %f y: %f  -- Keypoint train: x: %f y: %f \n"
    		, i, qp.x, qp.y , tp.x, tp.y ); 
    }

}





int main( int argc, char** argv){

  VideoCapture cap(0);
  if(!cap.isOpened())  // check if we succeeded
      return -1;	
  Mat frame;

/*  while(1){
      cap.read(frame); // get a new frame from camera
      DetectFeatureSURF(frame,400);
      //imshow("WebCamera", frame);
      if(waitKey(30) >= 0) break;
  }*/

  Mat img_1 = imread( argv[1], IMREAD_GRAYSCALE );
  Mat img_2 = imread( argv[2], IMREAD_GRAYSCALE );

  //DetectFeatureSURF(img_1,400);
  //DetectFeatureSIFT(img_1);
  //FeatureDescription(img_1,img_2);
  FeatureMatching_FLANN(img_1,img_2);
  waitKey(0);

 

	return 0;
}
