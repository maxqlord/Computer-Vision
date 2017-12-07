#include <cstdio>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void sobel(Mat src, Mat1s& dst, int direction)
{
    Mat1s kernel;
    int radius = 0;
    // Create the kernel
    if (direction == 0) {
        // Sobel 3x3 X kernel
        kernel = (Mat1s(3,3) << -1, 0, +1, -2, 0, +2, -1, 0, +1);
        radius = 1;
    } else {
        // Sobel 3x3 Y kernel
        kernel = (Mat1s(3, 3) << -1, -2, -1, 0, 0, 0, +1, +2, +1);
        radius = 1;
    }

    // Handle border issues
    Mat1b _src;
    copyMakeBorder(src, _src, radius, radius, radius, radius, BORDER_REFLECT101);

    // Create output matrix
    dst.create(src.rows, src.cols);

    // Convolution loop

    // Iterate on image
    for (int r = radius; r < _src.rows - radius; ++r)
    {
        for (int c = radius; c < _src.cols - radius; ++c)
        {
            short s = 0;

            // Iterate on kernel
            for (int i = -radius; i <= radius; ++i)
            {
                for (int j = -radius; j <= radius; ++j)
                {
                    s += _src(r + i, c + j) * kernel(i + radius, j + radius);
                }
            }
            dst(r - radius, c - radius) = s;
        }
    }
}

Mat edgeDetect(Mat src, int upperThreshold, int lowerThreshold, double size = 3) //thresholds are for derivative calcs
{

    //sobel(src, magX, 0);
    //sobel(src, magY, 1);

    //sobel holder
    Mat magX = Mat(src.rows, src.cols, CV_32F);
    Mat magY = Mat(src.rows, src.cols, CV_32F);

    //calculate sobel into magx and magy
    cv::Sobel(src, magX, CV_32F, 1, 0, (int)size);
    cv::Sobel(src, magY, CV_32F, 0, 1, (int)size);

    //calculate slope at all points- divide y derivative by x derivative
    Mat direction = Mat(src.rows, src.cols, CV_32F);
    cv::divide(magY, magX, direction);

    //magnitude of gradient at each pixel: sqrt((Gx)^2 + (Gy)^2)
    //create destination mats
    Mat sum = Mat(src.rows, src.cols, CV_64F);
    Mat prodX = Mat(src.rows, src.cols, CV_64F);
    Mat prodY = Mat(src.rows, src.cols, CV_64F);
    cv::multiply(magX, magX, prodX); //x derivative squared into prodx
    cv::multiply(magY, magY, prodY); //y derivative squared into prody
    sum = prodX + prodY; //x^2 + x^2
    cv::sqrt(sum, sum); //sqrt all elements in sum


    Mat returnImg = Mat(src.rows, src.cols, CV_8U); //instantiate image to output

    returnImg.setTo(cv::Scalar(0)); // Initialize image as a one channel image (filled with 0s)

    // Initialize iterators- magnitude, direction, and return iterators
    cv::MatIterator_<float> itMag = sum.begin<float>();
    cv::MatIterator_<double> itDirection = direction.begin<double>();

    cv::MatIterator_<unsigned char> itRet = returnImg.begin<unsigned char>();

    cv::MatIterator_<float> itend = sum.end<float>();

    for (; itMag != itend; ++itDirection, ++itRet, ++itMag) {   //loop through each pixel
        const cv::Point pos = itRet.pos(); //current pixel

        double currentDirection = atan(*itDirection) * 180 / 3.142; //gradient direction in degrees

        while (currentDirection < 0) currentDirection += 180; //make angle positive if necessary

        *itDirection = currentDirection; //store gradient angle

        if (*itMag < upperThreshold) continue; //if gradient magnitude is not high enough then skip

        bool flag = true; //edge or not - will be edge if it passes following tests

        if (currentDirection > 112.5 && currentDirection <= 157.5) { //edge from top left to bottom right
            if (pos.y > 0 && pos.x < src.cols - 1 && *itMag <= sum.at<float>(pos.y - 1, pos.x + 1))
                flag = false; //if magnitude lower than top right (w/ bound check)
            if (pos.y < src.rows - 1 && pos.x > 0 && *itMag <= sum.at<float>(pos.y + 1, pos.x - 1))
                flag = false; //if magnitude lower than bottom left (w/ bound check)
        } else if (currentDirection > 67.5 && currentDirection <= 112.5) {
            if (pos.y > 0 && *itMag <= sum.at<float>(pos.y - 1, pos.x)) flag = false;
            if (pos.y < src.rows - 1 && *itMag <= sum.at<float>(pos.y + 1, pos.x)) flag = false;
        } else if (currentDirection > 22.5 && currentDirection <= 67.5) {
            if (pos.y > 0 && pos.x > 0 && *itMag <= sum.at<float>(pos.y - 1, pos.x - 1)) flag = false;
            if (pos.y < src.rows - 1 && pos.x < src.cols - 1 && *itMag <= sum.at<float>(pos.y + 1, pos.x + 1))
                flag = false;
        } else {
            if (pos.x > 0 && *itMag <= sum.at<float>(pos.y, pos.x - 1)) flag = false;
            if (pos.x < src.cols - 1 && *itMag <= sum.at<float>(pos.y, pos.x + 1)) flag = false;
        }

        if (flag) { //if still an edge make white
            *itRet = 255;
            //printf("Edge");
        }
    }

    // Step 4: Hysterysis threshold
    bool imageChanged = true; //if image changed- will be set to true if new edge pixels found
    int i = 0; //counter
    while (imageChanged) {
        imageChanged = false;
        printf("Iteration %d", i);
        i++;
        itMag = sum.begin<float>();
        itDirection = direction.begin<double>();
        itRet = returnImg.begin<unsigned char>();
        itend = sum.end<float>();
        for (; itMag != itend; ++itMag, ++itDirection, ++itRet) {
            cv::Point pos = itRet.pos();
            if (pos.x < 2 || pos.x >= src.cols - 2 || pos.y < 2 || pos.y >= src.rows - 2)
                continue;
            float currentDirection = *itDirection;

            // Do we have a pixel we already know as an edge?
            if (*itRet == 255) {
                *itRet = (unsigned char) 64;
                if (currentDirection > 112.5 && currentDirection <= 157.5) {
                    if (pos.y > 0 && pos.x > 0) {
                        if (lowerThreshold <= sum.at<float>(pos.y - 1, pos.x - 1) &&
                            returnImg.at<unsigned char>(pos.y - 1, pos.x - 1) != 64 &&
                            direction.at<float>(pos.y - 1, pos.x - 1) > 112.5 &&
                            direction.at<float>(pos.y - 1, pos.x - 1) <= 157.5 &&
                            sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y - 2, pos.x) &&
                            sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y, pos.x - 2)) {
                            returnImg.ptr<unsigned char>(pos.y - 1, pos.x - 1)[0] = 255;
                            imageChanged = true;
                        }
                    }
                    if (pos.y < src.rows - 1 && pos.x < src.cols - 1) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x + 1, pos.y + 1)) &&
                            returnImg.at<unsigned char>(pos.y + 1, pos.x + 1) != 64 &&
                            direction.at<float>(pos.y + 1, pos.x + 1) > 112.5 &&
                            direction.at<float>(pos.y + 1, pos.x + 1) <= 157.5 &&
                            sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y + 2, pos.x) &&
                            sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y, pos.x + 2)) {
                            returnImg.ptr<unsigned char>(pos.y + 1, pos.x + 1)[0] = 255;
                            imageChanged = true;
                        }
                    }
                } else if (currentDirection > 67.5 && currentDirection <= 112.5) {
                    if (pos.x > 0) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x - 1, pos.y)) &&
                            returnImg.at<unsigned char>(pos.y, pos.x - 1) != 64 &&
                            direction.at<float>(pos.y, pos.x - 1) > 67.5 &&
                            direction.at<float>(pos.y, pos.x - 1) <= 112.5 &&
                            sum.at<float>(pos.y, pos.x - 1) > sum.at<float>(pos.y - 1, pos.x - 1) &&
                            sum.at<float>(pos.y, pos.x - 1) > sum.at<float>(pos.y + 1, pos.x - 1)) {
                            returnImg.ptr<unsigned char>(pos.y, pos.x - 1)[0] = 255;
                            imageChanged = true;
                        }
                    }
                    if (pos.x < src.cols - 1) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x + 1, pos.y)) &&
                            returnImg.at<unsigned char>(pos.y, pos.x + 1) != 64 &&
                            direction.at<float>(pos.y, pos.x + 1) > 67.5 &&
                            direction.at<float>(pos.y, pos.x + 1) <= 112.5 &&
                            sum.at<float>(pos.y, pos.x + 1) > sum.at<float>(pos.y - 1, pos.x + 1) &&
                            sum.at<float>(pos.y, pos.x + 1) > sum.at<float>(pos.y + 1, pos.x + 1)) {
                            returnImg.ptr<unsigned char>(pos.y, pos.x + 1)[0] = 255;
                            imageChanged = true;
                        }
                    }
                } else if (currentDirection > 22.5 && currentDirection <= 67.5) {
                    if (pos.y > 0 && pos.x < src.cols - 1) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x + 1, pos.y - 1)) &&
                            returnImg.at<unsigned char>(pos.y - 1, pos.x + 1) != 64 &&
                            direction.at<float>(pos.y - 1, pos.x + 1) > 22.5 &&
                            direction.at<float>(pos.y - 1, pos.x + 1) <= 67.5 &&
                            sum.at<float>(pos.y - 1, pos.x + 1) > sum.at<float>(pos.y - 2, pos.x) &&
                            sum.at<float>(pos.y - 1, pos.x + 1) > sum.at<float>(pos.y, pos.x + 2)) {
                            returnImg.ptr<unsigned char>(pos.y - 1, pos.x + 1)[0] = 255;
                            imageChanged = true;
                        }
                    }
                    if (pos.y < src.rows - 1 && pos.x > 0) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x - 1, pos.y + 1)) &&
                            returnImg.at<unsigned char>(pos.y + 1, pos.x - 1) != 64 &&
                            direction.at<float>(pos.y + 1, pos.x - 1) > 22.5 &&
                            direction.at<float>(pos.y + 1, pos.x - 1) <= 67.5 &&
                            sum.at<float>(pos.y + 1, pos.x - 1) > sum.at<float>(pos.y, pos.x - 2) &&
                            sum.at<float>(pos.y + 1, pos.x - 1) > sum.at<float>(pos.y + 2, pos.x)) {
                            returnImg.ptr<unsigned char>(pos.y + 1, pos.x - 1)[0] = 255;
                            imageChanged = true;
                        }
                    }
                } else {
                    if (pos.y > 0) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x, pos.y - 1)) &&
                            returnImg.at<unsigned char>(pos.y - 1, pos.x) != 64 &&
                            (direction.at<float>(pos.y - 1, pos.x) < 22.5 ||
                             direction.at<float>(pos.y - 1, pos.x) >= 157.5) &&
                            sum.at<float>(pos.y - 1, pos.x) > sum.at<float>(pos.y - 1, pos.x - 1) &&
                            sum.at<float>(pos.y - 1, pos.x) > sum.at<float>(pos.y - 1, pos.x + 2)) {
                            returnImg.ptr<unsigned char>(pos.y - 1, pos.x)[0] = 255;
                            imageChanged = true;
                        }
                    }
                    if (pos.y < src.rows - 1) {
                        if (lowerThreshold <= sum.at<float>(cv::Point(pos.x, pos.y + 1)) &&
                            returnImg.at<unsigned char>(pos.y + 1, pos.x) != 64 &&
                            (direction.at<float>(pos.y + 1, pos.x) < 22.5 ||
                             direction.at<float>(pos.y + 1, pos.x) >= 157.5) &&
                            sum.at<float>(pos.y + 1, pos.x) > sum.at<float>(pos.y + 1, pos.x - 1) &&
                            sum.at<float>(pos.y + 1, pos.x) > sum.at<float>(pos.y + 1, pos.x + 1)) {
                            returnImg.ptr<unsigned char>(pos.y + 1, pos.x)[0] = 255;
                            imageChanged = true;
                        }
                    }
                }
            }
        }
    }
    cv::MatIterator_<unsigned char> current = returnImg.begin<unsigned char>();
    cv::MatIterator_<unsigned char> final = returnImg.end<unsigned char>();
    for (; current != final; ++current) {
        if (*current == 64)
            *current = 255;
    }
    return returnImg;

        //Canny(src, edges, lowerThreshold, upperThreshold);
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

vector<Vec3f> houghTransform(Mat src, vector<Vec3f> circles) {
    HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8 );
    return circles;
}

double countMoney(vector<Vec3f> circles, Mat img) {
    double counter = 0;
    vector<double> radii;

    for( int i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        double radius = circles[i][2];
        printf("%d\t%d\t%f\n", center.x, center.y, radius);
        radii.push_back(radius);
        //circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        //circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
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
    blur = gaussBlur(gray); //apply gaussian blur to grayscale matrix and save into blur matrix
    //Canny(blur, edges, 50, 250);
    edges = edgeDetect(blur, 50, 30); //apply edge detection to blur matrix  bring upper higher
    namedWindow("edges", CV_WINDOW_NORMAL);
    imshow("edges", edges);
    waitKey(0);

    //circles = houghTransform(edges, circles); //apply hough transform to edge matrix and return list of circles
    //money = countMoney(circles, gray); //count the money from the circles
    //printf("Total: %f\n", money);



    return 0;

}