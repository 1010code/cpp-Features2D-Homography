#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
    Mat srcImage = imread("./images/target.jpg", 0);

//     int numFeatures = 20; // 可以指定擷取特徵數量 create(numFeatures)
//     Ptr<SIFT> dectector = SIFT::create();
//     Ptr<SURF> dectector = SURF::create();
    Ptr<ORB> dectector = ORB::create();

    vector<KeyPoint> keypoints;
    dectector->detect(srcImage, keypoints, Mat());
    printf("Total keypoints: %ld\n", keypoints.size());

    Mat keypoint_img;
    drawKeypoints(srcImage, keypoints, keypoint_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    cv::imwrite("result.jpg", keypoint_img);
}