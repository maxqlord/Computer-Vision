#include <cstdio>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat grayscale(Mat src) {


    Mat gray(src.rows, src.cols, CV_8U, Scalar(0));
    for(int row = 0; row < src.rows; row++) {
        const uchar *ptr = src.ptr(row);
        uchar *g = gray.ptr(row);
        for (int col = 0; col < src.cols; col++) {

            const uchar * uc_pixel = ptr;
            uchar * g_pixel = g;
            int a = uc_pixel[0];
            int b = uc_pixel[1];
            int c = uc_pixel[2];
            uchar grayVal = (uchar)(a*.07 + b*.72 + c*.21);
            g_pixel[0] = grayVal;
            ptr += 3;
            g += 1;

            //printf("%d\n", g[col]);
        }
    }


    //cvtColor(src, gray, CV_BGR2GRAY);
    return gray;


}

Mat gaussBlur(Mat src) {
    /*
     *  .01 .08 .01
     *  .08 .64 .08
     *  .01 .08 .01
     */
    Mat gauss(src.rows, src.cols, CV_8U, Scalar(0));

    for(int row = 1; row < src.rows - 1; row++) {
        for (int col = 1; col < src.cols -1; col++) {
            uchar topleft = src.at<uchar>(row-1,col-1);
            uchar topcenter = src.at<uchar>(row-1,col);
            uchar topright = src.at<uchar>(row-1,col+1);
            uchar middleleft = src.at<uchar>(row,col-1);
            uchar middlecenter = src.at<uchar>(row,col);
            uchar middleright = src.at<uchar>(row,col+1);
            uchar bottomleft = src.at<uchar>(row+1,col-1);
            uchar bottomcenter = src.at<uchar>(row+1,col);
            uchar bottomright = src.at<uchar>(row+1,col+1);
            uchar gaussval = (uchar)(topleft*.01+topcenter*.08+topright*.01+middleleft*.08+middlecenter*.64+middleright*.08+bottomleft*.01+bottomcenter*.08+bottomright*.01);
            gauss.at<uchar>(row, col) = gaussval;

            //printf("%d\n", g[col]);
        }
    }


    //GaussianBlur(src, dst, ksize, sigmaX, sigmaY);
    return gauss;
}

Mat sobel(Mat src, int angle)
{


    Mat sobel(src.rows, src.cols, CV_8U, Scalar(0));

    if(angle == 0) {
        for (int row = 1; row < src.rows - 1; row++) {
            for (int col = 1; col < src.cols - 1; col+=1) {
                uchar topleft = src.at<uchar>(row - 1, col - 1);
                uchar topcenter = src.at<uchar>(row - 1, col);
                uchar topright = src.at<uchar>(row - 1, col + 1);
                uchar middleleft = src.at<uchar>(row, col - 1);
                uchar middlecenter = src.at<uchar>(row, col);
                uchar middleright = src.at<uchar>(row, col + 1);
                uchar bottomleft = src.at<uchar>(row + 1, col - 1);
                uchar bottomcenter = src.at<uchar>(row + 1, col);
                uchar bottomright = src.at<uchar>(row + 1, col + 1);
                uchar sobelval = (uchar) (topleft * -1 + topcenter * 0 + topright * 1 + middleleft * -2 +
                                          middlecenter * 0 + middleright * 2 + bottomleft * -1 +
                                          bottomcenter * 0 + bottomright * 1);
                sobel.at<uchar>(row, col) = sobelval;

                //printf("%d\n", g[col]);
            }
        }
    } else {

        for (int row = 1; row < src.rows - 1; row++) {
            for (int col = 1; col < src.cols - 1; col+=1) {
                uchar topleft = src.at<uchar>(row - 1, col - 1);
                uchar topcenter = src.at<uchar>(row - 1, col);
                uchar topright = src.at<uchar>(row - 1, col + 1);
                uchar middleleft = src.at<uchar>(row, col - 1);
                uchar middlecenter = src.at<uchar>(row, col);
                uchar middleright = src.at<uchar>(row, col + 1);
                uchar bottomleft = src.at<uchar>(row + 1, col - 1);
                uchar bottomcenter = src.at<uchar>(row + 1, col);
                uchar bottomright = src.at<uchar>(row + 1, col + 1);
                uchar sobelval = (uchar) (topleft * -1 + topcenter * -2 + topright * -1 + middleleft * 0 +
                                          middlecenter * 0 + middleright * 0 + bottomleft * 1 +
                                          bottomcenter * 2 + bottomright * 1);
                sobel.at<uchar>(row, col) = sobelval;

                //printf("%d\n", g[col]);
            }
        }

    }


    //GaussianBlur(src, dst, ksize, sigmaX, sigmaY);
    sobel.convertTo(sobel, CV_32F); //1.0/255.0
    return sobel;
}

Mat edgeDetect(Mat src, int upper, int lower, double size = 3) //thresholds are for derivative calcs
{

    //sobel(src, magX, 0);
    //sobel(src, sobelY, 1);

    //sobel holder


    Mat sobelX = sobel(src, 1); //horizontal
    Mat sobelY = sobel(src, 0); //vertical


    //calculate sobel into magx and sobelY
    /*
    Mat sobelX = Mat(src.rows, src.cols, 1, CV_32F);  //horizontal
    Mat sobelY = Mat(src.rows, src.cols, 0, CV_32F);  //vertical
    cv::Sobel(src, sobelX, CV_32F, 1, 0, (int)size);
    cv::Sobel(src, sobelY, CV_32F, 0, 1, (int)size);
    */


    //calculate slope at all points- divide y derivative by x derivative
    Mat angle = Mat(src.rows, src.cols, CV_32F);
    cv::divide(sobelY, sobelX, angle);

    //magnitude of gradient at each pixel: sqrt((Gx)^2 + (Gy)^2)
    //create destination mats
    Mat sum = Mat(src.rows, src.cols, CV_64F);
    Mat dstX = Mat(src.rows, src.cols, CV_64F);
    Mat dstY = Mat(src.rows, src.cols, CV_64F);
    cv::multiply(sobelX, sobelX, dstX); //x derivative squared into dstX
    cv::multiply(sobelY, sobelY, dstY); //y derivative squared into dstY
    sum = dstX + dstY; //x^2 + x^2
    //sum.convertTo(sum, CV_64F, 1.0 / 255.0);
    //sum.convertTo(sum, CV_64F, 1.0 / 255.0);
    cv::sqrt(sum, sum); //sqrt all elements in sum   //DOES NOT WORK WITH 8U-convert to float
    /*namedWindow("mysobel", CV_WINDOW_NORMAL);
    imshow("mysobel", sum);
*/


    Mat finalImg = Mat(src.rows, src.cols, CV_8U); //instantiate image to output

    finalImg.setTo(cv::Scalar(0)); // Initialize image as a one channel image (filled with 0s)

    // Initialize iterators- magnitude, angle, and return iterators
    cv::MatIterator_<float> sobelIterator = sum.begin<float>();
    cv::MatIterator_<double> angleIterator = angle.begin<double>();

    cv::MatIterator_<unsigned char> finalIterator = finalImg.begin<unsigned char>();

    cv::MatIterator_<float> endIterator = sum.end<float>();

    for (; sobelIterator != endIterator; ++angleIterator, ++finalIterator, ++sobelIterator) {   //loop through each pixel
        const cv::Point p = finalIterator.pos(); //current pixel

        double currentangle = atan(*angleIterator) * 180 / 3.142; //gradient angle in degrees

        while (currentangle < 0) currentangle += 180; //make angle pitive if necessary

        *angleIterator = currentangle; //store gradient angle

        if (*sobelIterator < upper) continue; //if gradient magnitude is not high enough then skip

        bool isEdge = true; //edge or not - will be edge if it passes following tests

        if (currentangle > 112.5 && currentangle <= 157.5) { //edge from top left to bottom right
            if (p.y > 0 && p.x < src.cols - 1 && *sobelIterator <= sum.at<float>(p.y - 1, p.x + 1)) {
                isEdge = false; //if magnitude lower than top right (w/ bound check)
            }
            if (p.y < src.rows - 1 && p.x > 0 && *sobelIterator <= sum.at<float>(p.y + 1, p.x - 1)) {
                isEdge = false; //if magnitude lower than bottom left (w/ bound check)
            }

        } else if (currentangle > 67.5 && currentangle <= 112.5) {
            if (p.y > 0 && *sobelIterator <= sum.at<float>(p.y - 1, p.x)) {
                isEdge = false;
            }
            if (p.y < src.rows - 1 && *sobelIterator <= sum.at<float>(p.y + 1, p.x)) {
                isEdge = false;
            }

        } else if (currentangle > 22.5 && currentangle <= 67.5) {
            if (p.y > 0 && p.x > 0 && *sobelIterator <= sum.at<float>(p.y - 1, p.x - 1)) {
                isEdge = false;
            }
            if (p.y < src.rows - 1 && p.x < src.cols - 1 && *sobelIterator <= sum.at<float>(p.y + 1, p.x + 1)) {
                isEdge = false;
            }

        } else {
            if (p.x > 0 && *sobelIterator <= sum.at<float>(p.y, p.x - 1)) {
                isEdge = false;
            }
            if (p.x < src.cols - 1 && *sobelIterator <= sum.at<float>(p.y, p.x + 1)) {
                isEdge = false;
            }
        }

        if (isEdge) { //if still an edge make white
            *finalIterator = 255;
            //printf("Edge");
        }
    }
    //threshold
    // Step 4: Hysterysis threshold
    bool change = true; //if image changed- will be set to true if new edge pixels found
    int counter = 0; //counter
    while (change) {
        change = false;
        //printf("Iteration %d", i);
        counter++;
        //initialize iterators
        sobelIterator = sum.begin<float>();
        angleIterator = angle.begin<double>();
        finalIterator = finalImg.begin<unsigned char>();
        endIterator = sum.end<float>();
        for (; sobelIterator != endIterator; ++sobelIterator, ++angleIterator, ++finalIterator) {
            //get pixel gradient direction and position
            cv::Point p = finalIterator.pos();
            if (p.x < 2 || p.x >= src.cols - 2 || p.y < 2 || p.y >= src.rows - 2)
                continue;
            float currentangle = *angleIterator;

            // Do we have a pixel we already know as an edge?
            if (*finalIterator == 255) {
                //pixel already known to be an edge
                *finalIterator = (unsigned char) 64;


                /*
                The condition used in nonmaximum suppression still holds
                For (x-1, y-1) this means checking against (x, y-2) and (x-2, y)
                For (x+1, y+1) this means checking against the (x, y+2) and (x+2, y)
                 check if pixels (x-1, y-1) and (x+1, y+1) are edge pixels*/
                if (currentangle > 112.5 && currentangle <= 157.5) {
                    if (p.y > 0 && p.x > 0) {
                        //The gradient magnitude at these points is greater than the lower threshold
                        if (lower <= sum.at<float>(p.y - 1, p.x - 1) &&
                            finalImg.at<unsigned char>(p.y - 1, p.x - 1) != 64 && //hasn't already been checked
                            angle.at<float>(p.y - 1, p.x - 1) > 112.5 &&  //check if direction at pixel is same direction as currentangle
                            angle.at<float>(p.y - 1, p.x - 1) <= 157.5 &&
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y - 2, p.x) &&  //For (x-1, y-1) check against (x, y-2) and (x-2, y)
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y, p.x - 2)) {  // For (x+1, y+1) check against the (x, y+2) and (x+2, y)
                            finalImg.ptr<unsigned char>(p.y - 1, p.x - 1)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.y < src.rows - 1 && p.x < src.cols - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x + 1, p.y + 1)) &&
                            finalImg.at<unsigned char>(p.y + 1, p.x + 1) != 64 &&
                            angle.at<float>(p.y + 1, p.x + 1) > 112.5 &&
                            angle.at<float>(p.y + 1, p.x + 1) <= 157.5 &&
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y + 2, p.x) &&
                            sum.at<float>(p.y - 1, p.x - 1) > sum.at<float>(p.y, p.x + 2)) {
                            finalImg.ptr<unsigned char>(p.y + 1, p.x + 1)[0] = 255;
                            change = true;
                        }
                    }
                } else if (currentangle > 67.5 && currentangle <= 112.5) {
                    if (p.x > 0) {
                        if (lower <= sum.at<float>(cv::Point(p.x - 1, p.y)) &&
                            finalImg.at<unsigned char>(p.y, p.x - 1) != 64 &&
                            angle.at<float>(p.y, p.x - 1) > 67.5 &&
                            angle.at<float>(p.y, p.x - 1) <= 112.5 &&
                            sum.at<float>(p.y, p.x - 1) > sum.at<float>(p.y - 1, p.x - 1) &&
                            sum.at<float>(p.y, p.x - 1) > sum.at<float>(p.y + 1, p.x - 1)) {
                            finalImg.ptr<unsigned char>(p.y, p.x - 1)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.x < src.cols - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x + 1, p.y)) &&
                            finalImg.at<unsigned char>(p.y, p.x + 1) != 64 &&
                            angle.at<float>(p.y, p.x + 1) > 67.5 &&
                            angle.at<float>(p.y, p.x + 1) <= 112.5 &&
                            sum.at<float>(p.y, p.x + 1) > sum.at<float>(p.y - 1, p.x + 1) &&
                            sum.at<float>(p.y, p.x + 1) > sum.at<float>(p.y + 1, p.x + 1)) {
                            finalImg.ptr<unsigned char>(p.y, p.x + 1)[0] = 255;
                            change = true;
                        }
                    }
                } else if (currentangle > 22.5 && currentangle <= 67.5) {
                    if (p.y > 0 && p.x < src.cols - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x + 1, p.y - 1)) &&
                            finalImg.at<unsigned char>(p.y - 1, p.x + 1) != 64 &&
                            angle.at<float>(p.y - 1, p.x + 1) > 22.5 &&
                            angle.at<float>(p.y - 1, p.x + 1) <= 67.5 &&
                            sum.at<float>(p.y - 1, p.x + 1) > sum.at<float>(p.y - 2, p.x) &&
                            sum.at<float>(p.y - 1, p.x + 1) > sum.at<float>(p.y, p.x + 2)) {
                            finalImg.ptr<unsigned char>(p.y - 1, p.x + 1)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.y < src.rows - 1 && p.x > 0) {
                        if (lower <= sum.at<float>(cv::Point(p.x - 1, p.y + 1)) &&
                            finalImg.at<unsigned char>(p.y + 1, p.x - 1) != 64 &&
                            angle.at<float>(p.y + 1, p.x - 1) > 22.5 &&
                            angle.at<float>(p.y + 1, p.x - 1) <= 67.5 &&
                            sum.at<float>(p.y + 1, p.x - 1) > sum.at<float>(p.y, p.x - 2) &&
                            sum.at<float>(p.y + 1, p.x - 1) > sum.at<float>(p.y + 2, p.x)) {
                            finalImg.ptr<unsigned char>(p.y + 1, p.x - 1)[0] = 255;
                            change = true;
                        }
                    }
                } else {
                    if (p.y > 0) {
                        if (lower <= sum.at<float>(cv::Point(p.x, p.y - 1)) &&
                            finalImg.at<unsigned char>(p.y - 1, p.x) != 64 &&
                            (angle.at<float>(p.y - 1, p.x) < 22.5 ||
                             angle.at<float>(p.y - 1, p.x) >= 157.5) &&
                            sum.at<float>(p.y - 1, p.x) > sum.at<float>(p.y - 1, p.x - 1) &&
                            sum.at<float>(p.y - 1, p.x) > sum.at<float>(p.y - 1, p.x + 2)) {
                            finalImg.ptr<unsigned char>(p.y - 1, p.x)[0] = 255;
                            change = true;
                        }
                    }
                    if (p.y < src.rows - 1) {
                        if (lower <= sum.at<float>(cv::Point(p.x, p.y + 1)) &&
                            finalImg.at<unsigned char>(p.y + 1, p.x) != 64 &&
                            (angle.at<float>(p.y + 1, p.x) < 22.5 ||
                             angle.at<float>(p.y + 1, p.x) >= 157.5) &&
                            sum.at<float>(p.y + 1, p.x) > sum.at<float>(p.y + 1, p.x - 1) &&
                            sum.at<float>(p.y + 1, p.x) > sum.at<float>(p.y + 1, p.x + 1)) {
                            finalImg.ptr<unsigned char>(p.y + 1, p.x)[0] = 255;
                            change = true;
                        }
                    }
                }
            }
        }
    }
    cv::MatIterator_<unsigned char> current = finalImg.begin<unsigned char>();
    cv::MatIterator_<unsigned char> final = finalImg.end<unsigned char>();
    //switch already existing edge pixels back to edges
    for (; current != final; ++current) {
        if (*current == 64)
            *current = 255;
    }
    return finalImg;

        //Canny(src, edges, lower, upper);
}




vector<Vec3f> houghTransform(Mat src, vector<Vec3f> circles) {
    HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/16 );
    return circles;
}



double countMoney(vector<Vec3f> circles, Mat img) {
    double counter = 0;
    vector<double> radii;

    for( int i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        double radius = circles[i][2];
        //printf("%d\t%d\t%f\n", center.x, center.y, radius);
        radii.push_back(radius);
        //circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        //circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    for(int j = 0; j < radii.size(); j++) {
        if(radii[j] < 100) {
            counter += .1;
        } else if(radii[j] < 200) {
            counter += .01;
        } else if(radii[j] < 600) {
            counter += .05;
        } else if(radii[j] < 1000) {
            counter += .25;
        } else if(radii[j] < 600) {
            counter += 1;
        } else {
            counter += .5;
        }
    }

        /*
        // draw the circle center

        */

    return counter;
}
string type2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}

int main(int argc, char** argv )
{
    if ( argc != 2 ) //if command line arguments aren't correct
    {
        printf("usage: Coins.out <Image_Path>\n");
        return -1;
    }

    Mat gray, image, blur, edges, black; //declare image matrix and destination for grayscale matrix and blur matr
    vector<Vec3f> circles;
    double money;
    image = imread( argv[1], 1 ); //read image into image matrix
    if ( !image.data ) //if image is empty/does not exist
    {
        printf("No image data \n");
        return -1;
    }




    gray = grayscale(image); //convert image to grayscale and save into gray matrix
    string ty =  type2str( gray.type() );
    printf("Matrix: %s %dx%d \n", ty.c_str(), gray.cols, gray.rows );
    blur = gaussBlur(gaussBlur(gaussBlur(gaussBlur(gray)))); //apply gaussian blur to grayscale matrix and save into blur matrix
    //Canny(blur, edges, 50, 250);
    edges = edgeDetect(blur, 350 , 1000); //apply edge detection to blur matrix  bring upper higher   //50 30
    namedWindow("edges", CV_WINDOW_NORMAL);
    imshow("edges", edges);
    waitKey(0);
    //printf("test\n");
    circles = houghTransform(edges, circles); //apply hough transform to edge matrix and return list of circles
    //printf("test2\n");
    money = countMoney(circles, gray); //count the money from the circles
    //printf("test3\n");
    printf("Total: %f\n", money);



    return 0;

}