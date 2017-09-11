#include <iostream>
#include <fstream>
#include <cmath>



using namespace std;

    //draw triangle
    void draw_line(int M[700][700], int x1, int y1, int x2, int y2)
    {
        M[x1][y1] = 0;  //starting point
        int deltax = x2 - x1;  //delta
        int deltay = y2 - y1;  //delta

        int x_increment = (deltax > 0) - (deltax < 0);  //right or left - "bool" returns 1, 0, or -1 to get direction
        int y_increment = (deltay > 0) - (deltay < 0);  //up or down - "bool" returns 1, 0, or -1 to get direction

        deltax = std::abs(deltax) / 2; //xavg
        deltay = std::abs(deltay) / 2; //yavg

        if (deltax >= deltay) {//step by x
            int error = deltay - (deltax / 2);
            while (x1 != x2) { // reaches final point

                if ((error > 0) || (!error && (x_increment > 0))) { //there is error between correct and actual points or straight slope edge case
                    error -= deltax;
                    y1 += y_increment;
                }
                error += deltay;
                x1 += x_increment; //step right/left
                M[x1][y1] = 0; //
            }
        }
        else
        {
            int error = deltax - (deltay / 2);

            while (y1 != y2)
            {
                if ((error > 0) || (!error && (y_increment > 0)))  //there is error or 0 edge case
                {
                    error -= deltay;
                    x1 += x_increment;
                }
                error += deltax;
                y1 += y_increment;
                M[x1][y1] = 0;
            }
        }
    }




void print_vec(int M[700][700]) {
    ofstream myfile;
    myfile.open ("triangle.ppm");
    myfile << "P3  700  700  1 \n";
    for (int i = 0; i < 700; i++) {
        for (int j = 0; j < 700; j++) {
            myfile << M[j][i];
            myfile << " ";
            myfile << M[j][i];
            myfile << " ";
            myfile << M[j][i];
            myfile << " ";

        }
        myfile << "\n";
    }
    myfile.close();
    return;
}
float distance_formula(int x1, int y1, int x2, int y2) {
    return sqrtf((float)std::pow(x2-x1, 2) + (float)std::pow(y2-y1, 2));
}

void draw_circle(int M[700][700], float radius, int px, int py) {
    int x, y, xmax, y2, y2_new, ty;

    xmax = (int) (radius * 0.70710678); // maximum x at radius/sqrt(2)

    y = (int)radius;
    y2 = y * y;
    ty = (2 * y) - 1;
    y2_new = y2;

    for (x = 0; x <= xmax; x++) {
        if ((y2 - y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }

        M[px+x][py+y] = 0;
        M[px+x][py-y] = 0;
        M[px-x][py+y] = 0;
        M[px-x][py-y] = 0;
        M[px+y][py+x] = 0;
        M[px+y][py-x] = 0;
        M[px-y][py+x] = 0;
        M[px-y][py-x] = 0;

        y2_new -= (2 * x) - 3;

        /*
        if(px+x < 700 && py+y < 700) {   //not sure of the better way to check if in bounds with the different variables
            M[px+x][py+y] = 0;
        }
        if(px+x < 700 && py+y >= 0) {
            M[px+x][py-y] = 0;
        }
        if(px+x >= 0 && py+y < 700) {
            M[px-x][py+y] = 0;
        }
        if(px-x >= 0 && py-y >= 0) {
            M[px+x][py+y] = 0;
        }

        if(px+y < 700 && py+x < 700) {
            M[px+y][py+x] = 0;
        }
        if(px+y < 700 && py+x >= 0) {
            M[px+y][py-x] = 0;
        }
        if(px-y >= 0 && py+y < 700) {
            M[px-y][py+x] = 0;
        }
        if(px-y >= 0 && py-y >= 0) {
            M[px-y][py-x] = 0;
        }
        */

    }
}

/*
int GetCircumcenter(int x1, int y1, int x2, int y2, int x3, int y3) { //point struct would be better
    float D = 2 * (x1 * (y2 - y3) + x2 * (y3-y1) + x3 * (y1-y2));
    float Ix = (1/D) * ( (x1*x1 + y1*y1) * (y2 - y3)  +  (x2*x2 + y2*y2) * (y3 -y1) + (x3*x3 + y3*y3) * (y1 -y2) );
    float Iy = (1/D) * ( (x1*x1 + y1*y1) * (x3 - x2)  +  (x2*x2 + y2*y2) * (x1 - x3) + (x3*x3 + y3*y3) * (x2 - x1) );




}
*/



int main()
{
    srand(time(NULL));  //make sure random resets
    //fill white
    int M[700][700];
    for(int i=0; i<700; i++){
        for(int j=0; j<700; j++){
            M[i][j] = 1;
        }

    }

    //0-700 coordinates
    int x1 = rand() % 700;
    int y1 = rand() % 700;
    int x2 = rand() % 700;
    int y2 = rand() % 700;
    int x3 = rand() % 700;
    int y3 = rand() % 700;

    /*
    printf("%d\n", x1);
    printf("%d\n", y1);
    printf("%d\n", x2);
    printf("%d\n", y2);
    printf("%d\n", x3);
    printf("%d\n", y3);
     */
    //sides
    float a = distance_formula(x2,y2,x3,y3);
    float b = distance_formula(x1,y1,x3,y3);
    float c = distance_formula(x1,y1,x2,y2);

    /*
    printf("%f\n", a);
    printf("%f\n", b);
    printf("%f\n", c);
     */

    float s = (a+b+c)/2; //semiperimeter
    float r = (int)sqrtf(((s-a)*(s-b)*(s-c))/s); //incircle
    float R = (a*b*c)/(4*r*s); //circumcircle

    //incenter
    float Px = (a*(float)x1 + b*(float)x2 + c*(float)x3)/(a+b+c);
    float Py = (a*(float)y1 + b*(float)y2 + c*(float)y3)/(a+b+c);

    //circumcenter

    float D = 2 * (x1 * (y2 - y3) + x2 * (y3-y1) + x3 * (y1-y2));
    float Ix = (1/D) * ( (x1*x1 + y1*y1) * (y2 - y3)  +  (x2*x2 + y2*y2) * (y3 -y1) + (x3*x3 + y3*y3) * (y1 -y2) );
    float Iy = (1/D) * ( (x1*x1 + y1*y1) * (x3 - x2)  +  (x2*x2 + y2*y2) * (x1 - x3) + (x3*x3 + y3*y3) * (x2 - x1) );


    //M[Px][Py] = 0;

    //printf("%f\n", Px);
    //printf("%f\n", Py);
    //draw point on incenter and circumcenter
    //M[(int)Px][(int)Py] = 0;
    //M[(int)Ix][(int)Iy] = 0;
    //triangle
    draw_line(M,x1,y1,x2,y2);
    draw_line(M,x1,y1,x3,y3);
    draw_line(M,x2,y2,x3,y3);
    //circles
    draw_circle(M, r, (int)Px, (int)Py);

    draw_circle(M, R, (int)Ix, (int)Iy);

    print_vec(M);
    return 0;
}









