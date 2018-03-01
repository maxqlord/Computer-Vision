#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void print_vec(vector<Point2f> corners) {
    for(int i = 0; i < corners.size(); i++) {
        printf("X: %f\t Y: %f\n", corners[i].x, corners[i].y);
    }
}

int main(){

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    //VideoCapture cap("StructureFromMotionVideo.mp4");
    VideoCapture cap("withChessBoard.MOV");

    // Check if camera opened successfully
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while(true){

        Mat frame, frameGray;
        Size patternsize(7,7);
        vector<Point2f> corners;
        // Capture frame-by-frame
        cap >> frame;
        if (frame.empty()) {
            printf("empty\n");
            break;
        }
        //MOV coming in flipped both ways
        flip(frame, frame, 0);
        flip(frame, frame, +1);
        cvtColor(frame, frameGray, CV_BGR2GRAY);

        // If the frame is empty, break immediately


        bool patternfound = findChessboardCorners(frameGray, patternsize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);


        if(patternfound) {
            //printf("found\n");
            cornerSubPix(frameGray, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            circle(frame,Point((int)corners[24].x,(int)corners[24].y), 20, Scalar(0, 0, 255));
        } else {
            //printf("not found\n");
            circle(frame,Point((int)corners[26].x,(int)corners[26].y), 20, Scalar(0, 0, 255));
        }


        drawChessboardCorners(frame, patternsize, Mat(corners), patternfound);

        /*
         * on an 3x3 image
         *
         * 1    2   3
         * 4    5   6
         * 7    8   9
         *
         * The corners are read: 987654321
         */
        /*circle(frame,Point(1247,447), 10, Scalar(0, 0, 255));
        circle(frame,Point(1196,472), 10, Scalar(0, 0, 255));
        circle(frame,Point(1188,425), 10, Scalar(0, 0, 255));*/

        //circle(frame,Point((int)corners[24].x,(int)corners[24].y), 20, Scalar(0, 0, 255));

        // Display the resulting frame
        imshow( "Frame", frame );
        //print_vec(corners);

        // Press  ESC on keyboard to exit
        waitKey(200);


    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}