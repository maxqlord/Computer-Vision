#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <array>
#include <random>
#include <ctime>


int bestx1 = 0;
int besty1 = 0;
int bestx2 = 0;
int besty2 = 0;
float min_total = 30000000000;

struct point
{
    int x, y;
public:
    point (int,int);
};
using namespace std;

point::point (int a, int b) {
    x = a;
    y = b;
}

bool cmdx(const point & p1, const point & p2)
{
    return p1.x < p2.x;
}

bool cmdy(const point & p1, const point & p2)
{
    return p1.y < p2.y;
}

float distance_formula(int x1, int y1, int x2, int y2) {
    return sqrtf((float)std::pow(x2-x1, 2) + (float)std::pow(y2-y1, 2));
}

float brute_force_simple(vector<point> p, int n) //may need to return points
{
    float min = std::numeric_limits<float>::max();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            float dist;
            dist = distance_formula(p[i].x, p[i].y, p[j].x, p[j].y);
            if (dist < min) {
                min = dist;
                if(min < min_total) {
                    min_total = min;
                    bestx1 = p[i].x;
                    besty1 = p[i].y;
                    bestx2 = p[j].x;
                    besty2 = p[j].y;
                }

            }
        }
    }
    //printf("%s\t%d\t%d\t%d\t%d\t%f\n", "Best coordinates: ", bestx1, besty1, bestx2, besty2, min_total);
    return min;
}

float brute_force_one_point(vector<point> p, int n) //may need to return points
{
    float min = std::numeric_limits<float>::max();

    for (int j = 1; j < n; j++) {
        float dist;
        dist = distance_formula(p[0].x, p[0].y, p[j].x, p[j].y);
        if (dist < min) {
            min = dist;
            if(min < min_total) {
                min_total = min;
                bestx1 = p[0].x;
                besty1 = p[0].y;
                bestx2 = p[j].x;
                besty2 = p[j].y;
            }

        }
    }

    //printf("%s\t%d\t%d\t%d\t%d\t%f\n", "Best coordinates: ", bestx1, besty1, bestx2, besty2, min_total);
    return min;
}

float closest(vector<point> p, int n)
{
    // If there are 2 or 3 points, then use brute force
    if (n <= 3) {
        return brute_force_simple(p, n);
    }
    // Find the middle point
    int mid = n/2;
    point midPoint = p[mid];

    vector<point> left;
    vector<point> right;

    for(int i = 0; i < mid; i++) {
        left.push_back(p[i]);
    }
    for(int i = mid; i < p.size(); i++) {
        right.push_back(p[i]);
    }

    //sort(right.begin(), right.end(), cmdx);
    //sort(left.begin(), left.end(), cmdx);

    /* printf("left\n");
     for(int j = 0; j < left.size(); j++) {
         printf("%d\t%d\n", left[j].x, left[j].y);
     }
     printf("right\n");
     for(int i = 0; i < right.size(); i++) {
         printf("%d\t%d\n", right[i].x, right[i].y);
     }*/

    float dl = closest(left, mid);
    float dr = closest(right, n-mid);
    float d;
    if(dl < dr) {
        d = dl;
    } else {
        d = dr;
    }

    vector<point> strip;
    for (int i = 0; i < n; i++) {
        if(abs(p[i].x - midPoint.x) < d) {
            strip.push_back(p[i]);
        }
    }
    sort(strip.begin(), strip.end(), cmdy);

    /*for(int z = 0; z < strip.size(); z++) {
        printf("%d\t%d\n", strip[z].x, strip[z].y);
    }*/

    float strip_d = 3000000000;
    for (int j = 0; j < strip.size(); j+=1) { //j+=3
        vector<point> section = strip;
        section.erase(section.begin(), section.begin() + j);
        int points_left = (int)strip.size() - j;
        if(points_left > 6) {
            //printf("j: %d\t%d\n", j, (int)section.size());

            section.erase(section.begin() + 7, section.end());
            float distance_in_strip = brute_force_one_point(section, 7); //brute_force_simple
            if(distance_in_strip < strip_d) {
                strip_d = distance_in_strip;
            }
        }
        else  {
            if(points_left > 1) {
                float distance_in_strip = brute_force_one_point(section, points_left);
                if(distance_in_strip < strip_d) {
                    strip_d = distance_in_strip;
                }
            }
        }
    }
    //printf("%f\n", d);
    //printf("%f\n", strip_d);
    if (strip_d < d) {
        d = strip_d;
    }

    return d;
}

int main() {
    srand(time(NULL));


    //n log n
    for (int x = 100000; x <= 1000000; x += 100000) {
        std::clock_t start;
        double duration;
        start = std::clock();


        int num_of_points = x;
        int range_of_nums = 10*x;
        random_device rd;   // non-deterministic generator
        mt19937 gen(rd());  // to seed mersenne twister.
        uniform_int_distribution<> dist(0, range_of_nums);


        vector<point> p;

        p.reserve(num_of_points);
        for (int i = 0; i < num_of_points; i++) {
            p.push_back(point(dist(gen), dist(gen)));
        }
        /*
        printf("%s\n","Unsorted");
        for(int i = 0; i < p.size(); i++) {
            printf("%d\t%d\n", p[i].x, p[i].y);
        }*/


        sort(p.begin(), p.end(), cmdx);
/*
    printf("%s\n","Sorted");
    for(int i = 0; i < p.size(); i++) {
        printf("%d\t%d\n", p[i].x, p[i].y);
    }
*/
        int n = (int) p.size();
        float min = closest(p, n);
        duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        printf("O(n log n) algorithm\n\n");
        printf("The smallest distance is %f\n", min);
        printf("The closest coordinates were %d\t%d\t%d\t%d\n", bestx1, besty1, bestx2, besty2);


        //double time = duration/(double)1000;
        printf("Num of points: %d\t Time: %f\n", num_of_points, duration);

        bestx1 = 0;
        besty1 = 0;
        bestx2 = 0;
        besty2 = 0;
        min_total = 30000000000;
        p.clear();

    }

    printf("\n\nO(n^2) algorithm\n");
    //brute force
    for(int y = 5000; y <= 50000; y+= 5000) {
        std::clock_t start;
        double duration;
        start = std::clock();


        int num_of_points = y;
        int range_of_nums = 10*y;
        random_device rd;   // non-deterministic generator
        mt19937 gen(rd());  // to seed mersenne twister.
        uniform_int_distribution<> dist(0, range_of_nums);


        vector<point> p;

        p.reserve(num_of_points);
        for (int i = 0; i < num_of_points; i++) {
            p.push_back(point(rand() % range_of_nums, rand() % range_of_nums));
        }
        /*
        printf("%s\n","Unsorted");
        for(int i = 0; i < p.size(); i++) {
            printf("%d\t%d\n", p[i].x, p[i].y);
        }*/


        //sort(p.begin(), p.end(), cmdx);
/*
    printf("%s\n","Sorted");
    for(int i = 0; i < p.size(); i++) {
        printf("%d\t%d\n", p[i].x, p[i].y);
    }
*/
        int n = (int) p.size();
        float min = brute_force_simple(p, n);
        duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        printf("The smallest distance is %f\n", min);
        printf("The closest coordinates were %d\t%d\t%d\t%d\n", bestx1, besty1, bestx2, besty2);


        //double time = duration/(double)1000;
        printf("Num of points: %d\t Time: %f\n", num_of_points, duration);

        bestx1 = 0;
        besty1 = 0;
        bestx2 = 0;
        besty2 = 0;
        min_total = 30000000000;
        p.clear();
    }



    return 0;
}