#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	Mat img	= imread("lena.jpg");
	if (img.empty())
	{
		cout << "No image data \n" << endl;
		system("pause");
		return -1;
	}
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
	imshow("MyWindow", img);
	waitKey(0);
}
