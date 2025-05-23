#include<iostream>
#include<pthread.h>
#include<vector>
#include<chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;
#define INF 1000000
int n=5;

void floydWarshall(vector<vector<int>> &dist) {
  
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                dist[i][j] = dist[i][k] + dist[k][j];
            }
            }
        }
    }
}
void print(const vector<vector<int>> &dist) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (dist[i][j] == INF)
                cout<<"INF ";
            else
                cout<<dist[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){
    vector<vector<int>> dist(n, vector<int>(n, INF));//initialize a 2D vector with size n*n and initial value = INF
    dist={
        {0, 4, INF, 5, INF},
        {INF, 0, 1, INF, 6},
        {2, INF, 0, 3, INF},
        {INF, INF, 1, 0, 2},
        {1, INF, INF, 4, 0}
    };
    //initialize random values
    /*for (int i=0; i<n; i++) {
        dist[i][i]=0;
        for (int j=0; j<n;j++) {
            if (i != j) 
            dist[i][j]=rand() % 100 +1;
        }
    }*/
    auto start_seq = high_resolution_clock::now();
    floydWarshall(dist);
    print(dist);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq= duration_cast<microseconds>(end_seq - start_seq);
    double seconds=duration_seq.count()/1'000'000.0;

    cout<<"Sequential Floyd-Warshall Time: "<<fixed<< setprecision(6)<<seconds<<"s\n";
}
