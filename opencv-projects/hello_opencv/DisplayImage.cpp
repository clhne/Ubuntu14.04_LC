#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//int main( int argc, char** argv )
int main()
{
    Mat image;
    //image = imread( argv[1], 1 );
    image = imread("lena.jpg");

/*
    if( argc != 2 || !image.data )
    {
        cout<<"No image data \n";

        return -1;
    }
*/
    for(int i = 0; i < image.cols*image.rows * 3; i++)
	{
		image.data[i] /= 2;
	}
    namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
    imshow( "Display Image", image );
    imwrite("image_test.jpg",image);
	
    waitKey(0);

    return 0;
}
