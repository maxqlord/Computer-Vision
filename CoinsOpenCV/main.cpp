#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double countMoney(vector<Vec3f> circles, Mat img) {
    double counter = 0;
    vector<double> radii;
    double dimeVal = 120;
    double pennyVal = 140;
    double nickelVal = 150;
    double quarterVal = 165;
    double dollarVal = 190;
    double halfVal = 300;

    for( int i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        double radius = circles[i][2];
        //printf("%d\t%d\t%f\n", center.x, center.y, radius);
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

    Mat image, dst; //declare image matrix and destination for grayscale matrix and blur matrix

    vector<Vec3f> circles;
    double money;
    image = imread( argv[1], 1 ); //read image into image matrix
    if ( !image.data ) //if image is empty/does not exist
    {
        printf("No image data \n");
        return -1;
    }


    cvtColor(image, dst, CV_BGR2GRAY); //convert image to grayscale and save into gray matrix
    for(int i = 0; i < 1; i++) {
        GaussianBlur(dst, dst, Size(7,7), 7, 7); //apply gaussian blur to grayscale matrix and save into blur matrix
    }
    HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 1, 80, 30, 93, 100, 300);
    money = countMoney(circles, image); //count the money from the circles
    namedWindow("Circles", CV_WINDOW_NORMAL);
    imshow("Circles", image);
    waitKey(0);
    printf("Total: %f\n", money);



    return 0;

}
