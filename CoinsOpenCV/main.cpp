#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Mat edgeDetect(Mat src, Mat edges, int lowerThreshold, int upperThreshold) //thresholds are for derivative calcs
{
    //Sobel(src, src, CV_32F, 1, 0);
    Canny(src, edges, lowerThreshold, upperThreshold);
    return edges;
}

Mat gaussBlur(Mat src, Mat dst, Size ksize, double sigmaX, double sigmaY) {
    GaussianBlur(src, dst, ksize, sigmaX, sigmaY);
    return dst;
}

Mat grayscale(Mat src, Mat gray) {
    cvtColor(src, gray, CV_BGR2GRAY);
    return gray;
}

vector<Vec3f> houghTransform(Mat src, vector<Vec3f> circles) {
    HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8 );
    return circles;
}

double countMoney(vector<Vec3f> circles, Mat img) {
    double counter = 0;
    vector<double> radii;
    double dimeVal = 100;
    double pennyVal = 120;
    double nickelVal = 140;
    double quarterVal = 180;
    double dollarVal = 200;
    double halfVal = 300;

    for( int i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        double radius = circles[i][2];
        printf("%d\t%d\t%f\n", center.x, center.y, radius);
        radii.push_back(radius);
        circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );

        if(radius < dimeVal) {
            counter += .1;
        } else if(radius < pennyVal) {
            counter += .01;
        } else if(radius < nickelVal) {
            counter += .05;
        } else if(radius < quarterVal) {
            counter += .25;
        } else if(radius < dollarVal) {
            counter += 1;
        } else if(radius < halfVal) {
            counter += .5;
        }

    }

        /*
        // draw the circle center
        */

    return counter;
}

int main(int argc, char** argv )
{
    if ( argc != 2 ) //if command line arguments aren't correct
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image, gray, blur, edges, black; //declare image matrix and destination for grayscale matrix and blur matrix

    vector<Vec3f> circles;
    double money;
    image = imread( argv[1], 1 ); //read image into image matrix
    if ( !image.data ) //if image is empty/does not exist
    {
        printf("No image data \n");
        return -1;
    }


    gray = grayscale(image, gray); //convert image to grayscale and save into gray matrix
    blur = gaussBlur(gray, blur, Size(3,3), 0, 0); //apply gaussian blur to grayscale matrix and save into blur matrix
    edges = edgeDetect(blur, edges, 100, 200); //apply edge detection to blur matrix
    //namedWindow("edges", CV_WINDOW_NORMAL);
    //imshow("edges", edges);
    //waitKey(0);

    circles = houghTransform(edges, circles); //apply hough transform to edge matrix and return list of circles
    money = countMoney(circles, gray); //count the money from the circles
    printf("Total: %f\n", money);



    return 0;

}