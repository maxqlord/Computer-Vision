#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double countMoney(vector<Vec3f> circles, Mat img) {
    double counter = 0;
    vector<double> radii;
    double minR = 100000;
    double r;
    for( int j = 0; j < circles.size(); j++) {
        r = circles[j][2];
        if(r < minR) {
            minR = r;
        }
    }
    
    double dimeVal = minR;
    double pennyVal = 1.16*dimeVal;
    double nickelVal = 1.25*dimeVal;
    double quarterVal = 1.375*dimeVal;
    double dollarVal = 1.583*dimeVal;
    double halfVal = 2.5*dimeVal;

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
        printf("usage: ./CoinsOpenCV easy.jpg\n");
        return -1;
    }

    Mat image, dst, gray; //declare image matrix and destination for grayscale matrix and blur matrix

    vector<Vec3f> circles;
    double money;
    image = imread( argv[1], 1 ); //read image into image matrix
    if ( !image.data ) //if image is empty/does not exist
    {
        printf("No image data \n");
        return -1;
    }


    cvtColor(image, gray, CV_BGR2GRAY); //convert image to grayscale and save into gray matrix
        
    for(int i = 0; i < 1; i++) {
        GaussianBlur(gray, dst, Size(7,7), 7, 7); //apply gaussian blur to grayscale matrix and save into blur matrix
    }
    cvtColor(gray,gray,COLOR_GRAY2BGR);
    HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 1, 80, 30, 93, 100, 300);
    money = countMoney(circles, gray); //count the money from the circles
    namedWindow("Circles", CV_WINDOW_NORMAL);
    imshow("Circles", gray);
    waitKey(0);
    printf("Total: %f\n", money);



    return 0;

}
