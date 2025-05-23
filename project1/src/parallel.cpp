#include<iostream>
#include<pthread.h>
#include<chrono>
#include<vector>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define INF 1000000

int thread_num=6;
int n=300;
int chunk_size;
int rem;
int current_k;

vector<vector<int>> dist_parallel(n,vector<int>(n,INF));

void* floydWarshallParallel(void* arg){
    int th_id= *(int*)arg;
    int extra= th_id<rem ? th_id : rem;
    int start= th_id * chunk_size + extra;
    int end= start + chunk_size + (th_id<rem ? 1 : 0);

    for (int i = start; i < end; i++) {
        for (int j = 0; j < n; j++) {
            if (dist_parallel[i][current_k] + dist_parallel[current_k][j] < dist_parallel[i][j]) {
                dist_parallel[i][j] = dist_parallel[i][current_k] + dist_parallel[current_k][j];
            } 
        }
    }

    return NULL;
}
void print(const vector<vector<int>> &dist) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF)
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }}

int main(){
    //initialize matrix with known values
    /*dist_parallel = { 
        {0, 4, INF, 5, INF},
        {INF, 0, 1, INF, 6},
        {2, INF, 0, 3, INF},
        {INF, INF, 1, 0, 2},
        {1, INF, INF, 4, 0}
    }; */
    
    //initialize matrix with random values
   for(int i=0; i<n; i++){
        dist_parallel[i][i]=0;
        for(int j=0;j<n;j++){
            if(i !=j)
            dist_parallel[i][j]=rand()%100 +1;
        }
    }
    auto start= high_resolution_clock::now();
    pthread_t th[thread_num];
    int thread_id[thread_num];
    chunk_size=n/thread_num;
    rem=n%thread_num;

    for (int k=0;k<n;k++){
        current_k=k;
        for(int i=0;i<thread_num;i++){
            thread_id[i]=i;
            pthread_create(&th[i],NULL,floydWarshallParallel,&thread_id[i]);
        }
        for(int j=0;j<thread_num;j++)
            pthread_join(th[j],NULL);
        
    }
    //print(dist_parallel);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end-start);
    double seconds = duration.count() / 1'000'000.0;

    cout << "Parallel Floyd-Warshall Time: " << fixed << setprecision(6) << seconds << " s\n";

}