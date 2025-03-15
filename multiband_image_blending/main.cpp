#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>

using namespace std;
using namespace cv;

#define DEPTH 10

vector<Mat> GaussianPyramid(Mat img, int depth=5);
vector<Mat> LaplacianPyramid(Mat img, int depth=5);
Mat Reconstruct(const vector<Mat>& lPyramid);
Mat MultiBandBlending(const vector<Mat>& laplacianPyr1, const vector<Mat>& laplacianPyr2, const vector<Mat>& maskPyr);

int main() {
	Mat apple = imread("../img/apple.jpg");
	Mat orange = imread("../img/orange.jpg");
	Mat mask = imread("../img/mask.png");

	apple.convertTo(apple, CV_32F, 1/255.f);
	orange.convertTo(orange, CV_32F, 1 / 255.f);
	mask.convertTo(mask, CV_32F, 1 / 255.f);

	vector<Mat> appleLaplacianPyr = LaplacianPyramid(apple, DEPTH);
	vector<Mat> orangeLaplacianPyr = LaplacianPyramid(orange, DEPTH);
	vector<Mat> maskGaussianPyr = GaussianPyramid(mask, DEPTH);

	Mat result = MultiBandBlending(appleLaplacianPyr, orangeLaplacianPyr, maskGaussianPyr);

	imshow("Multi-band blending", result);
	waitKey();

	return 0;
}

vector<Mat> GaussianPyramid(Mat img, int depth) {
	vector<Mat> gpyramid;
	Mat src = img;

	for (int i = 0; i < depth; i++) {
		gpyramid.push_back(src);
		pyrDown(src, src);
	}
	return gpyramid;
}
vector<Mat> LaplacianPyramid(Mat img, int depth) {
	vector<Mat> lpyramid;
	auto gPyramid = GaussianPyramid(img, depth);

	for (int i = 0; i < depth-1; i++) {
		Mat upScaling, tmp;
		
		pyrUp(gPyramid[i+1], upScaling, gPyramid[i].size());
		tmp = gPyramid[i] - upScaling;
		lpyramid.push_back(tmp);
	}
	lpyramid.push_back(gPyramid.back());
	return lpyramid;
}
Mat Reconstruct(const vector<Mat>& lPyramid) {
	Mat ret = lPyramid.back();
	for (int i = lPyramid.size() - 2; i != -1; i--) {
		pyrUp(ret, ret, lPyramid[i].size());
		ret += lPyramid[i];
	}
	return ret;
}
Mat MultiBandBlending(const vector<Mat>& laplacianPyr1, const vector<Mat>& laplacianPyr2, const vector<Mat>& maskPyr) {
	vector<Mat> blendPyr;
	Mat maskLeft, maskRight;
	Mat imgLeft, imgRight, imgResult;

	for (int i = 0; i < DEPTH; i++) {
		maskLeft = maskPyr[i];
		maskRight = Scalar::all(1) - maskPyr[i];
		multiply(laplacianPyr1[i], maskLeft, imgLeft);
		multiply(laplacianPyr2[i], maskRight, imgRight);
		add(imgLeft, imgRight, imgResult);
		blendPyr.push_back(imgResult);
	}

	return Reconstruct(blendPyr);
}