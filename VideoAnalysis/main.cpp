#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main() {

    Mat frame, gray;

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    VideoCapture cap("StructureFromMotionVideo.mp4");

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    cap >> frame;
    while(!frame.empty()) {
        Size patternsize(8, 6); //interior number of corners
        cvtColor(frame, gray, CV_BGR2GRAY);
        vector<Point2f> corners;
        bool patternfound = findChessboardCorners(gray, patternsize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                                                  + CALIB_CB_FAST_CHECK);
        if (patternfound) {
            printf("found");
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        }
        drawChessboardCorners(frame, patternsize, Mat(corners), patternfound);

    }

    cap.release();
    destroyAllWindows();

    return 0;

}