#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Mat edgeDetect(Mat src, int upperThreshold, int lowerThreshold, double size = 3) //thresholds are for derivative calcs
{
    return src;
}

Mat gaussblur(Mat src, Mat dst, Size ksize, double sigmaX, double sigmaY) {
    GaussianBlur(src, dst, ksize, sigmaX, sigmaY);
    return dst;
}

Mat grayscale(Mat src, Mat gray) {
    cvtColor(src, gray, CV_BGR2GRAY);
    return gray;
}

int main(int argc, char** argv )
{
    if ( argc != 2 ) //if command line arguments aren't correct
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image, gray, blur; //declare image matrix and destination for grayscale matrix and blur matrix
    image = imread( argv[1], 1 ); //read image into image matrix
    if ( !image.data ) //if image is empty/does not exist
    {
        printf("No image data \n");
        return -1;
    }

    gray = grayscale(image, gray); //convert image to grayscale and save into gray matrix
    blur = gaussblur(gray, blur, Size(3,3), 0, 0); //apply gaussian blur to grayscale matrix and save into blur matrix


    namedWindow("blur+gray", CV_WINDOW_NORMAL);
    imshow("blur+gray", blur);
    waitKey(0);

    return 0;

}