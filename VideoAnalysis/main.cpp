#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{


    Mat camera_matrix;
    camera_matrix = (Mat_<double>(3, 3) << 1.2195112968898779e+003, 0., 3.6448211117862780e+002, 0.,
                1.2414409169216196e+003, 2.4321803868732076e+002, 0., 0., 1.);
    Mat coefficients = Mat::zeros(4,1,DataType<double>::type);

    vector<Point2f>corners;
    Mat rotation_vector;
    Mat translation_vector;


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
    Mat frame;
    Mat display;


    bool found=false;
    vector<Point3d> point3d;
    vector<Point2d> point2d;

    Size patternSize(7,7);

    vector<Point3f>  objectPoints;

    for (int j=0; j<patternSize.height;j++)
    {
        for( int i=0; i < patternSize.width;i++)
        {
            objectPoints.push_back(Point3f(i*4,j*4,0));
        }
    }


    point3d.push_back(Point3d(14, 2,-12.0));
    point3d.push_back(Point3d(14, 10,-12.0));
    point3d.push_back(Point3d(22, 10,-12.0));
    point3d.push_back(Point3d(22, 2,-12.0));
    cap >> frame;

    while(!frame.empty())
    {

        flip(frame, frame, 0);
        flip(display, display, 0);
        flip(frame, frame, +1);
        flip(display, display, +1);

        found = findChessboardCorners(frame, patternSize,  corners);


        if(found)
        {
            cvtColor(frame,display,CV_BGR2GRAY);
            cornerSubPix(display, corners, Size(11, 11), Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            //drawChessboardCorners(frame, patternSize, corners, found);

            solvePnP(objectPoints, corners, camera_matrix, coefficients, rotation_vector, translation_vector);

            projectPoints(point3d, rotation_vector, translation_vector, camera_matrix, coefficients, point2d);


            line(frame,corners[16],point2d[0],Scalar(0,0,255),6);
            line(frame,corners[30],point2d[1],Scalar(0,0,255),6);
            line(frame,corners[32],point2d[2],Scalar(0,0,255),6);
            line(frame,corners[18],point2d[3],Scalar(0,0,255),6);

            line(frame,corners[16],corners[18],Scalar(0,0,255),6);
            line(frame,corners[18],corners[32],Scalar(0,0,255),6);
            line(frame,corners[32],corners[30],Scalar(0,0,255),6);
            line(frame,corners[30],corners[16],Scalar(0,0,255),6);
            //32 30 18 16

            line(frame,point2d[0],point2d[1],Scalar(0,0,255),6);
            line(frame,point2d[1],point2d[2],Scalar(0,0,255),6);
            line(frame,point2d[2],point2d[3],Scalar(0,0,255),6);
            line(frame,point2d[3],point2d[0],Scalar(0,0,255),6);

        }

        imshow("Cube", frame);
        waitKey(1);
        cap >> frame;

    }

}