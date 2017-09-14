#include <iostream>
#include <fstream>
#include <cmath>



/*struct Point
{
    int x, y;
};*/
using namespace std;

void draw_line(int M[1000][1000], int x1, int y1, int x2, int y2)
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


float distance_formula(int x1, int y1, int x2, int y2) {
    return sqrtf((float)std::pow(x2-x1, 2) + (float)std::pow(y2-y1, 2));
}

int* brute_force(int pointsx[30], int pointsy[30], int num, int mycors[4]) {
    float min_distance = std::numeric_limits<float>::max();
    int best_i = 0;
    int best_j = 0;
    for(int i = 0; i < num; i++) {
        for (int j = i+1; j < num;j++) {
            float distance = distance_formula(pointsx[i], pointsy[i], pointsx[j], pointsy[j]);
            printf("%s\t%d\t%d\t %s\t%d\t%d\t %s\t%d\t%d\t %f\n", "I and J index:", i, j, "I X and Y Coordinates: ", pointsx[i],pointsy[i], "J X and Y Coordinates: ", pointsx[j],pointsy[j],  distance);
            if(distance < min_distance) {
                min_distance = distance;
                best_i = i;
                best_j = j;
            }
        }
    }
    int xcor1 = pointsx[best_i];
    int ycor1 = pointsy[best_i];
    int xcor2 = pointsx[best_j];
    int ycor2 = pointsy[best_j];
    mycors[0] = xcor1;
    mycors[1] = ycor1;
    mycors[2] = xcor2;
    mycors[3] = ycor2;



    printf("%s%d\n", "Best i:", best_i);
    printf("%s%d\n", "Best j:", best_j);
    printf("%s%f\n", "Min distance:", min_distance);
    return mycors;

}

void print_vec(int xlist[30], int ylist[30]) {
    int M[1000][1000];
    for(int x = 0; x < 1000; x++) {
        for(int y = 0; y < 1000; y++) {
            M[x][y] = 1;
        }
    }
    for(int z = 0; z < 30; z++) {
        printf("%d\t%d\t%d\n", z, xlist[z], ylist[z]);
        M[xlist[z]][ylist[z]] = 0;
    }
    int mycors[4] ={};
    int *arr = brute_force(xlist, ylist, 30, mycors);
    draw_line(M,arr[0], arr[1], arr[2],arr[3]);
    draw_line(M, arr[0], arr[1], arr[2], arr[1]);
    draw_line(M, arr[0], arr[1], arr[0], arr[3]);
    draw_line(M, arr[0], arr[3], arr[2], arr[3]);
    draw_line(M, arr[2], arr[1], arr[2], arr[3]);





    //printf("%s\n", "test");
    ofstream myfile;
    myfile.open("closest.ppm");
    myfile << "P3  1000  1000  1 \n";
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
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

}
int main() {

    srand(time(NULL));
    int xlist[30];
    int ylist[30];
    for(int i = 0; i < 30; i++) {
        xlist[i] = rand() % 1000;
        ylist[i] = rand() % 1000;
    }


    print_vec(xlist, ylist);

    //list = reinterpret_cast<Point[]>(generate_points(30));
    //float min_distance  = brute_force(xlist, ylist, 30);
    //printf("%f\n", min_distance);

    return 0;
}