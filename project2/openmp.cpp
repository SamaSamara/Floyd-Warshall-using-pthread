#include<iostream>
#include<omp.h>
#include<vector>
#include<chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define INF 1000000
int n=5;



vector<vector<int>> dist_parallel(n,vector<int>(n,INF));
vector<vector<int>> dist_seq(n,vector<int>(n,INF));

void printMatrix(const vector<vector<int>> &dist) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF)
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }}



void floydWarshall(vector<vector<int>> &dist) {
    for (int k=0;k<n;k++) {
        for (int i=0;i<n;i++) {
            for (int j=0; j<n; j++) {
                if (dist_seq[i][k] + dist_seq[k][j] < dist_seq[i][j]){
                dist_seq[i][j] = dist_seq[i][k] + dist_seq[k][j];
                }
            }
        }
    }
}

int main(){
    float speed;
    dist_seq= dist_parallel = {
        {0, 4, INF, 5, INF},
        {INF, 0, 1, INF, 6},
        {2, INF, 0, 3, INF},
        {INF, INF, 1, 0, 2},
        {1, INF, INF, 4, 0}
    };

    //initialize random values
    /*for (int i=0;i<n ;i++) {
        dist_parallel[i][i]= 0;
        dist_seq[i][i]= 0;

        for (int j = 0; j < n; j++) {
            if (i != j) 
            dist_parallel[i][j] = rand() % 100 +1;

        }
    }*/
    auto start = high_resolution_clock::now();

for (int k=0;k<n;k++) {
    #pragma openmp parallel for NUM_THREADS(6)
        for (int i=0;i<n;i++) {
            for (int j=0; j<n; j++) {
                if (dist_parallel[i][k] + dist_parallel[k][j] < dist_parallel[i][j]){
                dist_parallel[i][j] = dist_parallel[i][k] + dist_parallel[k][j];
                }
            }
        }
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    double seconds_par= duration.count() / 1'000'000.0;
    printMatrix(dist_parallel);
    cout<<"Exection time: "<< fixed << setprecision(6) << seconds_par<< " s\n";
    
}
