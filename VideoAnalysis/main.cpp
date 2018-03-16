#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;


struct CornerPoint {
    int index;
    float x;
    float y;
    float distance;
    public:
        CornerPoint (int,float, float, float);
};
bool distanceCheck (CornerPoint i,CornerPoint j) { return (i.distance <j.distance); }

CornerPoint::CornerPoint (int a, float b, float c, float d) {
    index = a;
    x = b;
    y = c;
    distance = d;
}

void print_vec(vector<Point2f> corners) {
    for(int i = 0; i < corners.size(); i++) {
        printf("X: %f\t Y: %f\n", corners[i].x, corners[i].y);
    }
}

float distance_formula(float x1, float y1, float x2, float y2) {
    return sqrtf((float)std::pow(x2-x1, 2) + (float)std::pow(y2-y1, 2));
}

vector<CornerPoint> corners_of_center(const Point &center, vector<Point2f> corners) {

    float min_distance = 1000000000;
    vector<CornerPoint> closestPoints;
    vector<Point2f> squareCorners;

    for(int i = 0; i < corners.size(); i++) {
        float distance = distance_formula(corners[i].x, corners[i].y, center.x, center.y);
        printf("%d\t%f\n", i, distance);
        if(closestPoints.size() < 2) {
            closestPoints.push_back(CornerPoint(i, corners[i].x, corners[i].y, distance));
        } else {
            if(distance < closestPoints[1].distance) {
                closestPoints.erase(closestPoints.begin()+2, closestPoints.end());
                closestPoints.push_back(CornerPoint(i, corners[i].x, corners[i].y, distance));
            }
        }
        sort(closestPoints.begin(), closestPoints.end(), distanceCheck);
    }

    for(int j = 0; j < closestPoints.size(); j++) {
        printf("%d\t%f\n", j, closestPoints[j].distance);
    }

    for(int k = 0; k < 2; k++) {
        printf("Side: %f\n", closestPoints[k].distance);
        squareCorners.push_back(Point((int)closestPoints[k].x, (int)closestPoints[k].y));
    }

    printf("FINISHED\n\n\n");

    /*
     * 0 1
     * 2 3
     */
    return closestPoints;


    //closestPoints now holds 9 closest points to center (including center)
}

int main(int argc, char** argv){

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    //VideoCapture cap("StructureFromMotionVideo.mp4");

    if ( argc != 2 ) //if command line arguments aren't correct
    {
        printf("usage: ./VideoAnalysis <Video_Path>\n");
        return -1;
    }



    VideoCapture cap(argv[1]); //withChessBoard.MOV

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

        //lower threshold?
        bool patternfound = findChessboardCorners(frameGray, patternsize, corners,
                                                  CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);

        Mat mat = (Mat_<double>(3, 3) << 1.2195112968898779e+003, 0., 3.6448211117862780e+002, 0., 0,1.2414409169216196e+003, 2.4321803868732076e+002, 0., 0.,1.);


        Mat dist_coeffs = Mat::zeros(4,1,DataType<double>::type); // Assuming no lens distortion

        vector<Point2f>corners1;// for chessboard pattern
        Mat rotation_vector; // Rotation in axis-angle form
        Mat translation_vector;

        vector<Point3f>  objectPoints;

        const string x= "X";
        const string y= "Y";
        const string z= "Z";
        bool found=false;
        vector<Point3d> point3D;
        vector<Point2d> point2D;

        int ChessboardPatternWidth=7;
        int ChessboardPatternHight=7;
        Size patternSize(ChessboardPatternWidth,ChessboardPatternHight);
        float BoardBoxSize=4;

        for (int j=0; j<patternSize.height;j++)
        {
            for( int i=0; i < patternSize.width;i++)
            {
                objectPoints.push_back(Point3f(i*BoardBoxSize,j*BoardBoxSize,0));
            }
        }


        point3D.push_back(Point3d(0,0,-10.0));
        point3D.push_back(Point3d(10.0,0,0));
        point3D.push_back(Point3d(0,10.0,0));


        point3D.push_back(Point3d(12, 0,-12.0));
        point3D.push_back(Point3d(12, 8,-12.0));
        point3D.push_back(Point3d(20, 8,-12.0));
        point3D.push_back(Point3d(20, 0,-12.0));





        if(patternfound) {
            //printf("found\n");
            cornerSubPix(frameGray, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

            solvePnP(objectPoints, corners1, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

            projectPoints(point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, point2D);


            line(frame,corners1[3],  point2D[3], Scalar(0,0,255),6);
            line(frame,corners1[21], point2D[4],Scalar(0,0,255), 6);
            line(frame,corners1[23], point2D[5],Scalar(0,0,255), 6);
            line(frame,corners1[5],  point2D[6], Scalar(0,0,255),6);

            line(frame,corners1[3],corners1[5], Scalar(0,255,0),  6);
            line(frame,corners1[5],corners1[23],Scalar(0,255,0) ,6);
            line(frame,corners1[23],corners1[21],Scalar(0,255,0),  6);
            line(frame,corners1[21],corners1[3],Scalar(0,255,0),   6);

            line(frame,point2D[3], point2D[4], Scalar(255,0,0), 6);
            line(frame,point2D[4], point2D[5], Scalar(255,0,0), 6);
            line(frame,point2D[5], point2D[6], Scalar(255,0,0), 6);
            line(frame,point2D[3], point2D[6], Scalar(255,0,0), 6);

            /*
            int closest_index = 24;
            circle(frame,Point((int)corners[closest_index].x,(int)corners[closest_index].y), 20, Scalar(0, 0, 255));
            line(frame, Point((int)corners[closest_index+8].x, (int)corners[closest_index+8].y),  Point((int)corners[closest_index+6].x, (int)corners[closest_index+6].y), Scalar(255,0,0), 10);
            line(frame, Point((int)corners[closest_index+8].x, (int)corners[closest_index+8].y),  Point((int)corners[closest_index-6].x, (int)corners[closest_index-6].y), Scalar(255,0,0), 10);
            line(frame, Point((int)corners[closest_index-6].x, (int)corners[closest_index-6].y),  Point((int)corners[closest_index-8].x, (int)corners[closest_index-8].y), Scalar(255,0,0), 10);
            line(frame, Point((int)corners[closest_index+6].x, (int)corners[closest_index+6].y),  Point((int)corners[closest_index-8].x, (int)corners[closest_index-8].y), Scalar(255,0,0), 10);
*/
            //This is camera matrix and dist_coeffs , you will get in camera calibration.(copy from intrinsic.xml)



        }
            //printf("not found\n");
            //find average of all points and select point as center which is closest to the average of them
            /*int center;
            float x_total = 0;
            float y_total = 0;
            for(int i = 0; i < corners.size(); i++) {
                x_total += corners[i].x;
                y_total += corners[i].y;
            }
            float avg_x = x_total/corners.size();
            float avg_y = y_total/corners.size();

            int closest_index = 0;
            float min_distance = 1000000000;
            for(int i = 0; i < corners.size(); i++) {
                float distance = distance_formula(corners[i].x, corners[i].y, avg_x, avg_y);
                if(distance < min_distance) {
                    min_distance = distance;
                    closest_index = i;
                }
            }
             //green

            float x_distance = corners[closest_index-1].x - corners[closest_index].x;
            float y_distance = -1* (corners[closest_index-1].y - corners[closest_index].y);
            float distance = sqrtf(x_distance*x_distance + y_distance*y_distance);
            Point corner1 = Point((int)corners[closest_index].x,(int)corners[closest_index].y);
            Point corner2 = Point((int)corners[closest_index-1].x,(int)corners[closest_index-1].y);
            float angle = cvFastArctan(y_distance, x_distance); //angle from center to anchor point
            printf("%f\n",angle);

            float angle_to_3 = 270+angle;
            if(angle_to_3 > 360) {
                angle_to_3 -= 360;
            }

            Point corner3 = Point((int)corners[2].x,(int)corners[2].y);


            circle(frame,corner1, 20, Scalar(0, 0, 255)); //red
            circle(frame,corner2, 20, Scalar(255, 0, 0)); //blue
            circle(frame,corner3, 20, Scalar(0, 255, 0)); //green


            printf("Center X,Y %f\t%f\tClosest X,Y %f\t%f\t X_d,Y_d,T_d %f\t%f\t%f\n", corners[closest_index].x, corners[closest_index].y, corners[closest_index-1].x, corners[closest_index-1].y, x_distance, y_distance, distance);
            */

            /*vector<CornerPoint> squareCorners= corners_of_center(Point((int)corners[closest_index].x,(int)corners[closest_index].y), corners);

            circle(frame,Point((int)squareCorners[1].x,(int)squareCorners[1].y), 20, Scalar(0, 0, 255)); //red
            circle(frame,Point((int)squareCorners[2].x,(int)squareCorners[2].y), 20, Scalar(0, 255, 0)); //green
            */



            /*
            line(frame, Point((int)squareCorners[0].x, (int)squareCorners[0].y),  Point((int)squareCorners[1].x, (int)squareCorners[1].y), Scalar(255,0,0), 10);
            line(frame, Point((int)squareCorners[0].x, (int)squareCorners[0].y),  Point((int)squareCorners[2].x, (int)squareCorners[2].y), Scalar(255,0,0), 10);
            line(frame, Point((int)squareCorners[1].x, (int)squareCorners[1].y),  Point((int)squareCorners[3].x, (int)squareCorners[3].y), Scalar(255,0,0), 10);
            line(frame, Point((int)squareCorners[2].x, (int)squareCorners[2].y),  Point((int)squareCorners[3].x, (int)squareCorners[3].y), Scalar(255,0,0), 10);
            */
        /*
         * 0 1
         * 2 3
         */


        //drawChessboardCorners(frame, patternsize, Mat(corners), patternfound);

        // Display the resulting frame
        imshow( "Frame", frame );
        //print_vec(corners);

        // Press  ESC on keyboard to exit
        waitKey(1);


    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}

        /*
         * on an 3x3 image
         *
         * 1    2   3
         * 4    5   6
         * 7    8   9
         *
         *
         * indices:
         *
         * 48 47 46 45 44 43 42
         * 41 40 39 38 37 36 35
         * 34 33 32 31 30 29 28
         * 27 26 25 24 23 22 21
         * 20 19 18 17 16 15 14
         * 13 12 11 10 9  8  7
         * 6  5  4  3  2  1  0
         *
         *
         */

        /*
           int middleRight = closest_index - 1;
           int middleLeft = closest_index + 1;
           float lineDistance = distance_formula(corners[middleRight].x, corners[middleRight].y, corners[middleLeft].x, corners[middleLeft].y);
           float cornerDistance = lineDistance/2.0f;

           Point topRight = Point((int)corners[middleRight].x, (int)(corners[middleRight].y - cornerDistance));
           Point bottomRight = Point((int)corners[middleRight].x, (int)(corners[middleRight].y + cornerDistance));
           Point topLeft = Point((int)corners[middleLeft].x, (int)(corners[middleLeft].y - cornerDistance));
           Point bottomLeft = Point((int)corners[middleLeft].x, (int)(corners[middleLeft].y + cornerDistance));
           line(frame, topRight, topLeft, Scalar(255,0,0), 10);
           line(frame, topRight, bottomRight, Scalar(255,0,0), 10);
           line(frame, bottomRight, bottomLeft, Scalar(255,0,0), 10);
           line(frame, topLeft, bottomLeft, Scalar(255,0,0), 10);


           line(frame, Point((int)corners[closest_index+8].x, (int)corners[closest_index+8].y),  Point((int)corners[closest_index+6].x, (int)corners[closest_index+6].y), Scalar(255,0,0), 10);
           line(frame, Point((int)corners[closest_index+8].x, (int)corners[closest_index+8].y),  Point((int)corners[closest_index-6].x, (int)corners[closest_index-6].y), Scalar(255,0,0), 10);
           line(frame, Point((int)corners[closest_index-6].x, (int)corners[closest_index-6].y),  Point((int)corners[closest_index-8].x, (int)corners[closest_index-8].y), Scalar(255,0,0), 10);
           line(frame, Point((int)corners[closest_index+6].x, (int)corners[closest_index+6].y),  Point((int)corners[closest_index-8].x, (int)corners[closest_index-8].y), Scalar(255,0,0), 10);
       */
        /*circle(frame,Point(1247,447), 10, Scalar(0, 0, 255));
        circle(frame,Point(1196,472), 10, Scalar(0, 0, 255));
        circle(frame,Point(1188,425), 10, Scalar(0, 0, 255));*/

        //circle(frame,Point((int)corners[24].x,(int)corners[24].y), 20, Scalar(0, 0, 255));

