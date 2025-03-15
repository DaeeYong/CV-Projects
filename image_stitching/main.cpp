#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/features2d.hpp>
#include<opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat img1 = imread("../img/img1.jpg");
	Mat img2 = imread("../img/img2.jpg");

	vector<KeyPoint> keypoints1, keypoints2; // for saving feature points
	Mat descriptor1, descriptor2; //for saving descriptor value
	Ptr<SIFT> detector = SIFT::create(0,3,0.09,10, 1.6); //detector for extracting feature points using SIFT 

	//dectect feature points (feature point == keypoint)
	detector->detect(img1, keypoints1);
	detector->detect(img2, keypoints2);
	
	//compute descriptor
	detector->compute(img1, keypoints1, descriptor1);
	detector->compute(img2, keypoints2, descriptor2);
	
	/*
	detector->detectAndCompute(img1, noArray(), keypoints1, descriptor_img1);
	detector->detectAndCompute(img2, noArray(), keypoints2, descriptor_img2);
	*/
	
	Ptr<BFMatcher> matcher = BFMatcher::create(NORM_L2, 1); //NORM_L1 : (Xi-mean), NORM_L2 : (Xi-mean)^2

	vector <DMatch> matches;
	matcher->match(descriptor1,descriptor2,matches);

	sort(matches.begin(), matches.end());
	vector<DMatch> good_matches(matches.begin(), matches.begin() + 50);

	Mat good_match_img;
	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, good_match_img, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//imshow("feature point match", good_match_img);
	//waitKey();

	vector<Point2f> query_img;
	vector<Point2f> train_img;

	for (int i = 0; i < good_matches.size(); i++) {
		query_img.push_back(keypoints1[good_matches[i].queryIdx].pt);
		train_img.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	//homography matrix using RANSA
	Mat homography = findHomography(train_img, query_img, RANSAC);
			
	Mat hImg2;
	warpPerspective(img2, hImg2, homography, Size(img1.cols*2, img2.rows*2), INTER_CUBIC); //bicubic(interpolation)

	Mat panorama;
	panorama = hImg2.clone();

	Mat matTmp(panorama, Rect(0, 0, img1.cols, img1.rows)); //shallow copy
	img1.copyTo(matTmp);

	imshow("panorama", panorama);
	imshow("img1", img1);
	imshow("img2", img2);
	waitKey();
	return 0;
}