#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

int sigma =2;
vector<Mat> coins;
vector<Mat> blurCoins;

void ReadImage();

int main() {

	ReadImage();
	
	for (int i = 0; i < coins.size(); i++) {
		Mat dst;
		GaussianBlur(coins[i], dst, Size(), sigma);
		blurCoins.push_back(dst);
	}
	for (int i = 0; i < blurCoins.size(); i++) {
		cvtColor(blurCoins[i], blurCoins[i], COLOR_BGR2GRAY);
	}
	for (int i = 0; i < blurCoins.size(); i++) {
		int cnt = 0;
		vector<Vec3f> circles;
		HoughCircles(blurCoins[i], circles, HOUGH_GRADIENT, 1, 35, 40, 60, 10, 90);
		for (auto vec : circles) {
			cnt += 1;
			Point center(cvRound(vec[0]), cvRound(vec[1]));
			int radius = vec[2];
		}
		cout << "coins" << i << " counts: " << cnt << '\n';
		imshow("cur img", blurCoins[i]);
		waitkey();
	}

	return 0;
}

void ReadImage() {
	Mat coins0 = imread("../img/coins0.jpg");
	Mat coins1 = imread("../img/coins1.jpg");
	Mat coins2 = imread("../img/coins2.jpg");
	Mat coins3 = imread("../img/coins3.jpg");
	Mat coins4 = imread("../img/coins4.jpg");
	Mat coins5 = imread("../img/coins5.jpg");

	coins.push_back(coins0);
	coins.push_back(coins1);
	coins.push_back(coins2);
	coins.push_back(coins3);
	coins.push_back(coins4);
	coins.push_back(coins5);
}
