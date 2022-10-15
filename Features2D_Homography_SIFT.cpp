#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <string>

using namespace cv;
using std::cout;
using std::endl;
using std::string;

int main()
{
    int MIN_MATCH_COUNT = 10;
    bool matchesMask = false;
    string object_path = "./images/object.jpg";
    string scene_path = "./images/target.jpg";
    Mat img_object = imread(object_path, 0);
    Mat img_scene = imread(scene_path, 0);
    if ( img_object.empty() || img_scene.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
    }
    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 400;
    Ptr<SIFT> detector = SIFT::create(minHessian);
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute( img_object, noArray(), keypoints_object, descriptors_object );
    detector->detectAndCompute( img_scene, noArray(), keypoints_scene, descriptors_scene );
    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors_object, descriptors_scene, knn_matches, 2 );
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.75f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    /*** 判斷 Match 數量是否有達門檻 */
    cout << "Good matches: " << good_matches.size() << endl;
    if(MIN_MATCH_COUNT < good_matches.size()){
        //-- Draw matches
        Mat img_output = imread(scene_path, 1);
        //-- Localize the object
        std::vector<Point2f> obj;
        std::vector<Point2f> scene;
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
        }
        Mat H = findHomography( obj, scene, RANSAC );
        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = Point2f(0, 0);
        obj_corners[1] = Point2f( (float)img_object.cols, 0 );
        obj_corners[2] = Point2f( (float)img_object.cols, (float)img_object.rows );
        obj_corners[3] = Point2f( 0, (float)img_object.rows );
        std::vector<Point2f> scene_corners(4);
        perspectiveTransform( obj_corners, scene_corners, H);
        line( img_output, scene_corners[2], scene_corners[3], Scalar( 0, 255, 0), 4 );
        //-- Show detected matches
        cv::imwrite("result.jpg", img_output);
        matchesMask = true;
    }
    if(matchesMask){
        cout << "Good! find" << endl;
    }
}