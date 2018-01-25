#include <cstdio>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int accumulator[5000][5000];

Mat grayscale(Mat src) {

    Mat gray = src.clone();

    for (int x = 0; x < src.cols; x++) {
        for (int y = 0; y < src.rows; y++) {
            Vec3b color = src.at<Vec3b>(Point(x, y));
            uchar grayVal = (uchar) ((color[0] + color[1] + color[2]) / 3);
            color[0] = grayVal;
            color[1] = grayVal;
            color[2] = grayVal;
            gray.at<Vec3b>(Point(x, y)) = color;

        }
    }

    return gray;
}

Mat gaussBlur(Mat src) {
    /*
     *  .01 .08 .01
     *  .08 .64 .08
     *  .01 .08 .01
     */
    Mat gauss = src.clone();

    double weights[3][3] = {
            {.01,.08,.01},
            {.08,.64,.08},
            {.01,.08,.01}
    };

    for(int x = 1; x < src.cols-1; x++) {

        for (int y = 1; y < src.rows-1; y++) {

            int sum = 0;
            for (int a = -1; a <= 1; a++) {

                for (int b = -1; b <= 1; b++) {
                    Vec3b pixel = src.at<Vec3b>(Point(x+a,y+b));
                    uchar blurval = (uchar)(pixel[0] * weights[a+1][b+1]);
                    sum += blurval;

                }
            }
            Vec3b color = src.at<Vec3b>(Point(x,y));
            if(sum > 255) {
                sum = 255;
            }
            color[0] = uchar(sum);
            color[1] = uchar(sum);
            color[2] = uchar(sum);
            gauss.at<Vec3b>(Point(x,y)) = color;
        }
    }
    return gauss;
}

Mat sobelAngle(Mat src) {
    Mat sobel = src.clone();

    double horizonalWeights[3][3] = {
            {1, 0, -1},
            {2, 0, -2},
            {1, 0, -1}
    };
    double verticalWeights[3][3] = {
            {-1, -2, -1},
            {0,  0,  0},
            {1,  2,  1}
    };


    for (int x = 1; x < src.cols - 1; x++) {

        for (int y = 1; y < src.rows - 1; y++) {

            double horizontalSum = 0;
            double verticalSum = 0;

            for (int a = -1; a <= 1; a++) {

                for (int b = -1; b <= 1; b++) {
                    Vec3b pixel = src.at<Vec3b>(Point(x + a, y + b));
                    horizontalSum += (pixel[0] * horizonalWeights[a + 1][b + 1]);
                    verticalSum += (pixel[0] * verticalWeights[a + 1][b + 1]);

                }
            }
            //double sum = sqrt(horizontalSum * horizontalSum + verticalSum * verticalSum);
            double proportion = verticalSum/horizontalSum;
            Vec3b color = src.at<Vec3b>(Point(x, y));

            color[0] = uchar(proportion);
            color[1] = uchar(proportion);
            color[2] = uchar(proportion);
            sobel.at<Vec3b>(Point(x, y)) = color;
        }
    }
    return sobel;
}

Mat sobelOperator(Mat src) {
    Mat sobel = src.clone();

    double horizonalWeights[3][3] = {
            {1,0,-1},
            {2,0,-2},
            {1,0,-1}
    };
    double verticalWeights[3][3] = {
            {-1,-2,-1},
            {0,0,0},
            {1,2,1}
    };

    for(int x = 1; x < src.cols-1; x++) {

        for (int y = 1; y < src.rows-1; y++) {

            int horizontalSum = 0;
            int verticalSum = 0;

            for (int a = -1; a <= 1; a++) {

                for (int b = -1; b <= 1; b++) {
                    Vec3b pixel = src.at<Vec3b>(Point(x+a,y+b));
                    horizontalSum += (pixel[0] * horizonalWeights[a+1][b+1]);
                    verticalSum += (pixel[0] * verticalWeights[a+1][b+1]);

                }
            }
            double sum = sqrt(horizontalSum*horizontalSum + verticalSum*verticalSum);
            double angle = cvFastArctan(verticalSum, horizontalSum);
            Vec3b color = src.at<Vec3b>(Point(x,y));
            if(sum > 255) {
                sum = 255;
            }
            color[0] = uchar(sum);
            color[1] = uchar(sum);
            color[2] = uchar(sum);
            sobel.at<Vec3b>(Point(x,y)) = color;
        }
    }
    return sobel;

}


Mat edgeDetect(Mat src, Mat angle, int upper, int lower, double size = 3) //thresholds are for derivative calcs
{

    Mat sum = src;

    Mat finalImg = Mat(src.rows, src.cols, CV_8U); //instantiate image to output

    finalImg.setTo(cv::Scalar(0)); // Initialize image as a three channel image (filled with 0s)

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
        else {
            *finalIterator = 0;
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
    Mat output;
    cvtColor(finalImg,output,COLOR_GRAY2BGR);
    return output;

        //Canny(src, edges, lower, upper);
}


/*
Mat convertToThree(Mat src) {

    Mat output;
    ;
    for(int x = 0; x < src.cols; x++) {
        for (int y = 0; y < src.rows; y++) {
            printf("test\n");
            Vec3b color = src.at<Vec3b>(Point(x, y));
            printf("test1\n");
            uchar channelCol = color[0];
            printf("test2\n");
            color[1] = channelCol;
            printf("test3\n");
            color[2] = channelCol;

            output.at<Vec3b>(Point(x,y)) = color;
            printf("test4\n");
        }
    }
    return output;
}*/

string type2str(int type) {  //detects number of channels and data type- https://stackoverflow.com/questions/10167534
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

bool checkHistory(int x, int y, int r, vector<int> vx, vector<int> vy, vector<int> vr, int threshold) {
    /*printf("%d %d %d\n", x, y, r);
    for(int a = 0; a < vx.size(); a++) {
        printf("%d\n",vx[a]);
    }*/
    for(int i = 0; i < vx.size(); i++) {
        for(int j = 0; j < vy.size(); j++) {
            for(int k = 0; k < vr.size(); k++) {
                //printf("%d %d %d\n", abs(x-vx[i]), abs(y-vy[j]), abs(r-vr[k]));
                if(abs(x-vx[i]) < threshold && abs(y-vy[j]) < threshold && r-vr[k] < 30) {
                    return false;
                }
            }
        }
    }
    return true;

}


Mat houghTransform(Mat image, Mat src, int circleThreshold, int radiusSize, int distanceThreshold) { //200

    Mat houghInput;
    vector<int> vx;
    vector<int> vy;
    vector<int> vr;
    cvtColor(src, houghInput, COLOR_BGR2GRAY);
    Mat hough= image.clone();
    vector<Vec3f> circles;
    //zero



    for(int radius = 35; radius < radiusSize; radius++) {
        for (int i = 0; i < 5000; i++) {
            for(int j = 0; j < 5000; j++) {
                accumulator[i][j] = 0;
            };
        };
        //printf("%d\n", radius);
        for(int x = 1; x < src.cols-1; x++) {
            for (int y = 1; y < src.rows - 1; y++) {
                if(src.at<Vec3b>(Point(x,y))[0] == 255) {
                    for(int theta = 0; theta <= 360; theta++) {
                        int a = (int)(x - radius * cos(theta*(3.14159/180.0)));  //column
                        int b = (int)(y - radius * sin(theta*(3.14159/180.0)));  //row


                        if(b > 0 && b < src.rows && a > 0 && a < src.cols && src.at<Vec3b>(Point(a,b))[0] == 255){
                            accumulator[x][y]++;
                            //printf("%d\n", accumulator[a][b]);
                            //printf("%d %d\n", a, b);
                        }


                    }

                }
            }
        }

        //printf("%d %d\n",src.cols, src.rows);
        for(int innerx = 1; innerx < src.cols-1; innerx++) {
            for (int innery = 1; innery < src.rows - 1; innery++) {
                //printf("%d %d\n", innerx, innery);
                /*if(innerx == 734 && innery == 628) {
                    printf("%d\n", accumulator[innerx][innery]);
                }*/


                if(accumulator[innerx][innery] > circleThreshold && checkHistory(innerx, innery, radius, vx, vy, vr, distanceThreshold)) {
                    //printf("point %d %d\n", innerx, innery);
                    vx.push_back(innerx);
                    vy.push_back(innery);
                    vr.push_back(radius);
                    hough.at<Vec3b>(Point(innerx,innery))[0] = 0;
                    hough.at<Vec3b>(Point(innerx,innery))[1] = 0;
                    hough.at<Vec3b>(Point(innerx,innery))[2] = 255;

                    //printf("x %d y %d r %d\n", innerx, innery, radius);

                    for(int theta = 0; theta <= 360; theta++) {
                        int a = (int)(innerx - radius * cos(theta*(3.14159/180.0)));
                        int b = (int)(innery - radius * sin(theta*(3.14159/180.0)));
                        if(a > 0 && a < src.cols && b > 0 && b < src.rows){

                            hough.at<Vec3b>(Point(a,b))[0] = 0;
                            hough.at<Vec3b>(Point(a,b))[1] = 0;
                            hough.at<Vec3b>(Point(a,b))[2] = 255;
                            //printf("%d\n", accumulator[a][b]);
                            //printf("%d %d\n", a, b);
                        }

                    }

                }

            }
        }

    }
    HoughCircles( houghInput, circles, CV_HOUGH_GRADIENT, 1, src.rows/14, 30, 23, 20, 60 );  //get the remaining ones
    for( int i = 0; i < circles.size(); i++ ) {
        //printf("%d\n", i);
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radiusHough = cvRound(circles[i][2]);
        hough.at<Vec3b>(center)[0] = 0;
        hough.at<Vec3b>(center)[1] = 0;
        hough.at<Vec3b>(center)[2] = 255;
        vr.push_back(radiusHough);

        for(int theta = 0; theta <= 360; theta++) {
            int a = (int)(center.x - radiusHough * cos(theta*(3.14159/180.0)));
            int b = (int)(center.y - radiusHough * sin(theta*(3.14159/180.0)));
            if(a > 0 && a < src.cols && b > 0 && b < src.rows){

                hough.at<Vec3b>(Point(a,b))[0] = 0;
                hough.at<Vec3b>(Point(a,b))[1] = 0;
                hough.at<Vec3b>(Point(a,b))[2] = 255;
                //printf("%d\n", accumulator[a][b]);
                //printf("%d %d\n", a, b);
            }

        }
    }
    sort(vr.begin(),vr.end());
    double total = 0;

    for(int f = 0; f < vr.size(); f++) {
        printf("%d\n", vr[f]);
        if(vr[f] < .27) {
            total += .1;
        } else if(vr[f] < 33) {
            total += .01;
        } else if(vr[f] < 35) {
            total += .05;
        } else if(vr[f] < 38) {
            total += .25;
        } else if(vr[f] < 4) {
            total += 1;
        } else {
            total += .5;
        }
    }
    printf("Total: %.2f\n", total);
    return hough;

}




int main(int argc, char** argv )
{
    if ( argc != 2 ) //if command line arguments aren't correct
    {
        printf("usage: Coins.out <Image_Path>\n");
        return -1;
    }

    Mat gray, image, blur, sobel, angle, edges, hough; //declare image matrix and destination for grayscale matrix and blur matr
    vector<Vec3f> circles;
    double money;
    image = imread( argv[1], 1 ); //read image into image matrix
    if ( !image.data ) //if image is empty/does not exist
    {
        printf("No image data \n");
        return -1;
    }

    //Size size(image.cols/4,image.rows/4);//the dst image size,e.g.100x100
    Mat dst;//dst image
    resize(image,dst,Size(), .2, .2);//resize image


    Mat output,output2;

    printf("Grayscale\n");
    gray = grayscale(dst); //convert image to grayscale and save into gray matrix

    //string ty =  type2str( gray.type() );
    //printf("Matrix: %s %dx%d \n", ty.c_str(), gray.cols, gray.rows );
    printf("Blur\n");
    blur = gaussBlur(gaussBlur(gaussBlur(gaussBlur(gaussBlur(gaussBlur(gaussBlur(gray)))))));

    printf("Sobel Magnitude\n");
    sobel = sobelOperator(blur);

    printf("Sobel Angle\n");
    angle = sobelAngle(blur);

    printf("Canny\n");
    edges = edgeDetect(sobel, angle, 225 , 225);

    printf("Hough Transform\n");
    hough = houghTransform(dst, edges, 110, 100, 50);

    //resize(hough, output2, Size(), .5,.5);
    namedWindow("Hough", CV_WINDOW_NORMAL);
    imshow("Hough", hough);
    waitKey(0);


    return 0;

}