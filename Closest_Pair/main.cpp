#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <array>
#include <random>
#include <unordered_map>


int bestx1 = 0;
int besty1 = 0;
int bestx2 = 0;
int besty2 = 0;
float min_total = 30000000000;


struct point
{
    int x, y, identifer;
    /*
    bool operator()(point& p1, const point& p2)
    {
        if(p1.x == p2.x && p2.x == p2.y) {
            return true;
        }
        return false;
    }*/

public:
        point (int,int);
};
using namespace std;


point::point (int a, int b) {
    x = a;
    y = b;
    identifer = rand();
}

bool cmdx(const point & p1, const point & p2) {
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

    for (int j = 0; j < p.size(); j++) {
        float dist;
        if(n!=j) {
            dist = distance_formula(p[n].x, p[n].y, p[j].x, p[j].y);
            if (dist < min) {
                min = dist;
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

int random_index(int size) {
    random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<> dist(0, size-1);
    return (dist(gen));
}

vector<point> addedVector(vector<point> neighborhood, vector<point> toBeAdded) {
    int end = (int)toBeAdded.size();
    for(int i = 0; i < end; i++) {
        neighborhood.push_back(toBeAdded[i]);
    }
    return neighborhood;
}

vector<point> makeNeighborhood(int row, int col, unordered_map<int,vector<point> > map, int size) {
    vector<point> neighborhood;

    if(row < size && row > -1 && col < size && col > -1) {
        int index = row * size + col;
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }



    if(row-1 < size && row-1 > -1 && col-1 < size && col-1 > -1) {
        int index = (row-1)*size + (col-1);
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }

    if(row-1 < size && row-1 > -1 && col < size && col > -1) {
        int index = (row-1)*size + col;
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }

    if(row-1 < size && row-1 > -1 && col+1 < size && col+1 > -1) {
        int index = (row-1)*size + (col+1);
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }




    if(row < size && row > -1 && col-1 < size && col-1 > -1) {
        int index = row*size + (col-1);
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }

    if(row < size && row > -1 && col+1 < size && col+1 > -1) {
        int index = row*size + (col+1);
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }




    if(row+1 < size && row+1 > -1 && col-1 < size && col-1 > -1) {
        int index = (row+1)*size + (col-1);
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }

    if(row+1 < size && row+1 > -1 && col < size && col > -1) {
        int index = (row+1)*size + col;
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }

    if(row+1 < size && row+1 > -1 && col+1 < size && col+1 > -1) {
        int index = (row+1)*size + (col+1);
        vector<point> cell_list = map[index];
        neighborhood = addedVector(neighborhood, cell_list);
    }

    /*printf("Neighborhood row %d\t Neighborhood col %d\n", row, col);
    for(int x = 0; x < neighborhood.size(); x++) {
        printf("neighbor x %d\t neighbor y %d\n", neighborhood[x].x, neighborhood[x].y);
    }
    printf("\n");
     */
    return neighborhood;

}

/*
bool hasPointInCell(int row, int col, unordered_map<int,vector<point> > map, int size) {
    int index = row*size + col;
    return !map[index].empty();

}
 */
bool pointInMap(int index, unordered_map<int,vector<point> > map) {
    if(index > -1) {
        return !map[index].empty();
    }
    return false;

}

bool hasPointInNeighborhood(int row, int col, unordered_map<int,vector<point> > map, int size) {

    //index
    int index = row*size+col;
    if(map[index].size() > 1) {
        return true;
    }
    //index + 1
    if(pointInMap(index+1, map)) {
        return true;
    }
    //index - 1
    if(pointInMap(index-1, map)) {
        return true;
    }
    //index + size -1
    if(pointInMap(index+size-1, map)) {
        return true;
    }
    //index + size
    if(pointInMap(index+size, map)) {
        return true;
    }
    //index + size +1
    if(pointInMap(index+size+1, map)) {
        return true;
    }
    //index - size -1
    if(pointInMap(index-size-1, map)) {
        return true;
    }
    //index - size
    if(pointInMap(index-size, map)) {
        return true;
    }
    //index - size +1
    if(pointInMap(index-size+1, map)) {
        return true;
    }
    return false;


    //return makeNeighborhood(row, col, std::move(map), size).size() > 1;
}


float closest_in_neighborhood(point p, int row, int col, unordered_map<int,vector<point> > map, int size) {

    vector<point> neighborhood = map[row*size+col];

    float closest_dist = std::numeric_limits<float>::max();
    int end = (int)neighborhood.size();
    for(int i = 0; i < end; i++) {
        if(p.identifer != neighborhood[i].identifer) {
            float dist = distance_formula(p.x, p.y, neighborhood[i].x, neighborhood[i].y);
            if (dist < closest_dist) {
                closest_dist = dist;
            }
        }
    }
    //one = p
    //two = neighborhood[i]
    return closest_dist;
}






float sieve_algorithm(vector<point> p, int square_length){



    //build mesh anchored at top left
    //build dictionary with key being (r,c) and value being a list of all points in that cell
    //7x7 maps to a 0-48 array- 7*r + c
    //then make hashing structure- unordered map in C++
    //coalesce hashing


    vector<point> set = p;
    int counter = 0;
    float dist_final;
    float b;

    //unordered_map<tuple<int,int>,vector<point> > map;


    while(!set.empty()) {
        int size;
        unordered_map<int,vector<point> > map;
        p = set;
        int index = random_index((int)set.size());
        //point xi = set.at((unsigned long)index);
        point xi = set[index];
        float dxi = std::numeric_limits<float>::max(); //closest point to xi
        //dxi = closest(set, (int)set.size());
        //float test_brute = brute_force_one_point(set, index);
        dxi = brute_force_one_point(set, index);
        //printf("%s\n", "past dxi");
        //dxi is now distance from xi to closest point in set

        b = dxi/3.0f; //length of cell in mesh
        //printf("dxi %f\tb %f\n", dxi, b);

        dist_final = dxi;
        double width_d = (double)square_length / b;
        size = (int)width_d + 1;


        int end = (int) set.size();
        for (int i = 0; i < end; i++) {
            int row = (int) (((float) set[i].x )/ b);
            int col = (int) (((float) set[i].y )/ b);
            int map_index = row * size + col;
            map[map_index].push_back(set[i]);
            vector<point> a = map[map_index];
            //printf("index %d\t map_index %d\t pointx %d\t pointy %d\t row %d\t, col %d\n", i, map_index,
            //map[map_index][0].x, map[map_index][0].y, row, col);
            /*if(counter == 0) {

            }*/
            //map[std::make_tuple(row,col)].push_back(set[i]);
        }

        //printf("dxi %f\tb %f\tsize %d\n", dxi, b, size);
        //printf(map[std::make_tuple(0,0)]);



        //implement using hashing- remove points with no points in neighborhood
        for(int j = (int)set.size()-1; j > -1; j--) {
            //int row = (int)(((float)set[j].x)/b);
           // int col = (int)(((float)set[j].y)/b);
            int row = 1;
            int col = 1;


            printf("%d\n", j);
            //bool test = hasPointInNeighborhood(row, col, map, size);
            if(!hasPointInNeighborhood(row, col, map, size)) {
                set.erase(set.begin() + j);
                //int map_index = row*size + col;
                //map.erase(map_index);
            }
        }

    counter +=1; //maybe not last round
    }
    //counter is i*
    //dist_final is dxi*

    printf("Ended\n");

    b = 3*dist_final;
    printf("Dist final: %f\n", b);
    double width_d = (double)square_length / b;
    int size = (int)width_d + 1;
    unordered_map<int,vector<point> > map_final;
    int end = (int) p.size();
    for (int i = 0; i < end; i++) {
        int row = (int) (((float) p[i].x) / b);
        int col = (int) (((float) p[i].y )/ b);
        int map_index = row * size + col;
        //printf("index %d\t map_index %d\t pointx %d\t pointy %d\t row %d\t, col %d\n", i, map_index, p[i].x, p[i].y, row, col);
        map_final[map_index].push_back(p[i]);
        vector<point> a = map_final[map_index];
        //map[std::make_tuple(row,col)].push_back(set[i]);
    }

    float closest_dist = std::numeric_limits<float>::max();
    float closest_neighborhood;
    for(int i = 0; i < end ; i++) {
        int row = (int)(((float)p[i].x)/b);
        int col = (int)(((float)p[i].y)/b);
        //float test_neighborhood = closest_in_neighborhood(p[i], row, col, map_final, size);
        closest_neighborhood = closest_in_neighborhood(p[i], row, col, map_final, size);
        if(closest_neighborhood < closest_dist) { //closest pair
            closest_dist = closest_neighborhood;
        }
    }

    return closest_dist;
}




int main() {
    //printf("%s\n", "did something");
    //clock_t time = std::clock();
    //srand((int)time);
    srand(time(NULL));

    for (int x = 1000; x <= 10000; x += 1000) {
        std::clock_t start;
        double duration;
        start = std::clock();


        int num_of_points = x;
        int range_of_nums = x;
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
        printf("%s\n", "algorithm started");
        float min = sieve_algorithm(p, range_of_nums);
        duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        printf("The smallest distance is %f\n", min);
        //printf("The closest coordinates were %d\t%d\t%d\t%d\n", bestx1, besty1, bestx2, besty2);


        //double time = duration/(double)1000;
        printf("Num of points: %d\t Time: %f\n", num_of_points, duration);

        bestx1 = 0;
        besty1 = 0;
        bestx2 = 0;
        besty2 = 0;
        min_total = 30000000000;
        p.clear();

    }

















    //printf("%d", (int)time);

    //n log n
    /*
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

        printf("%s\n","Unsorted");
        for(int i = 0; i < p.size(); i++) {
            printf("%d\t%d\n", p[i].x, p[i].y);
        }


        sort(p.begin(), p.end(), cmdx);

    printf("%s\n","Sorted");
    for(int i = 0; i < p.size(); i++) {
        printf("%d\t%d\n", p[i].x, p[i].y);
    }

        int n = (int) p.size();
        float min = closest(p, n, range_of_nums);
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
*/



    //printf("\n\n\n");






    //printf("\n\n\n");


     /*
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

    */


    return 0;
}
