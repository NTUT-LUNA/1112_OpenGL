#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

	String ImgPath = "C:/Users/luna/Documents/NTUT/Year2_Sem2/Vision/HW/HW1/images/Lena.png";
	Mat image = imread(ImgPath, CV_LOAD_IMAGE_COLOR);

	if (!image.data) {
		cout << "Can not load the image!" << std::endl;
		return -1;
	}
	namedWindow("Display Window", CV_WINDOW_NORMAL);
	imshow("Display Window", image);
	waitKey(0);
	return 0;
}
