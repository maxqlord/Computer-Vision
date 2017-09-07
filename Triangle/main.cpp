#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <cmath>

using namespace std;

    // Bresenham's line algorithm
    void draw_line(int M[50][50], int x1, int y1, int const x2, int const y2)
    {
        int delta_x(x2 - x1);
        // if x1 == x2, then it does not matter what we set here
        signed char const ix = (delta_x > 0) - (delta_x < 0);  //right or left
        delta_x = std::abs(delta_x) / 2;

        int delta_y(y2 - y1);
        // if y1 == y2, then it does not matter what we set here
        signed char const iy = (delta_y > 0) - (delta_y < 0);  //up or down
        delta_y = std::abs(delta_y) / 2;

        M[x1][y1] = 0;

        if (delta_x >= delta_y)
        {
            // error may go below zero
            int error = delta_y - (delta_x / 2);

            while (x1 != x2)
            {
                // reduce error, while taking into account the corner case of error == 0
                if ((error > 0) || (!error && (ix > 0)))
                {
                    error -= delta_x;
                    y1 += iy;
                }
                // else do nothing

                error += delta_y;
                x1 += ix;

                M[x1][y1] = 0;
            }
        }
        else
        {
            // error may go below zero
            int error = delta_x - (delta_y / 2);

            while (y1 != y2)
            {
                // reduce error, while taking into account the corner case of error == 0
                if ((error > 0) || (!error && (iy > 0)))
                {
                    error -= delta_y;
                    x1 += ix;
                }
                // else do nothing

                error += delta_x;
                y1 += iy;

                M[x1][y1] = 0;
            }
        }
    }




void print_vec(int M[50][50]) {
    ofstream myfile;
    myfile.open ("triangle.ppm");
    myfile << "P3  50  50  1 \n";
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            myfile << M[i][j];
            myfile << " ";
            myfile << M[i][j];
            myfile << " ";
            myfile << M[i][j];
            myfile << " ";

        }
        myfile << "\n";
    }
    myfile.close();
    return;
}


int main()
{
    srand(time(NULL));
    //vector<vector<int> > M(30, vector<int>(30,1));
    int M[50][50];
    for(int i=0; i<50; i++){
        for(int j=0; j<50; j++){
            M[i][j] = 1;
        }

    }

    int x1 = rand() % 40 + 10;
    int y1 = rand() % 40 + 10;
    int x2 = rand() % 40 + 10;
    int y2 = rand() % 40 + 10;
    int x3 = rand() % 40 + 10;
    int y3 = rand() % 40 + 10;

    printf("%d\n", x1);
    printf("%d\n", y1);
    printf("%d\n", x2);
    printf("%d\n", y2);
    printf("%d\n", x3);
    printf("%d\n", y3);


    draw_line(M,x1,y1,x2,y2);
    draw_line(M,x1,y1,x3,y3);
    draw_line(M,x2,y2,x3,y3);

    print_vec(M);


    return 0;
}




