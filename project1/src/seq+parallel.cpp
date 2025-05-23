#include<iostream>
#include<pthread.h>
#include<vector>
#include<chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define INF 1000000


int thread_num=4;
int n=300;
int chunk_size;
int rem;
int current_k;

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

void* floydWarshallParallel(void* arg){
    int th_id = *(int*)arg;
    int extra = th_id<rem ? th_id : rem;
    int start = th_id * chunk_size + extra;
    int end = start + chunk_size + (th_id < rem ? 1 : 0);

    for (int i = start; i < end; i++) {
        for (int j = 0; j < n; j++) {
            if (dist_parallel[i][current_k] + dist_parallel[current_k][j] < dist_parallel[i][j]) {
                dist_parallel[i][j] = dist_parallel[i][current_k] + dist_parallel[current_k][j];
            } 
        }
    }

    return NULL;
}

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
    cout<< "number of threads: "<<thread_num<<"   N="<<n<<endl;
    /*dist_seq= dist_parallel = {
        {0, 4, INF, 5, INF},
        {INF, 0, 1, INF, 6},
        {2, INF, 0, 3, INF},
        {INF, INF, 1, 0, 2},
        {1, INF, INF, 4, 0}
    };*/

    //initialize random values
    for (int i=0;i<n ;i++) {
        dist_parallel[i][i]= 0;
        dist_seq[i][i]= 0;

        for (int j = 0; j < n; j++) {
            if (i != j) 
            dist_parallel[i][j] = rand() % 100 +1;
            dist_seq[i][j] = dist_parallel[i][j];

        }
    }
    //sequential start
    auto start_seq = high_resolution_clock::now();
    floydWarshall(dist_seq);
    //cout<<"\n seq after \n";
    //printMatrix(dist_seq);//print results
    auto end_seq = high_resolution_clock::now();
    auto duration_seq= duration_cast<microseconds>(end_seq - start_seq);
    double seconds= duration_seq.count() / 1'000'000.0;

    cout << "Sequential Floyd-Warshall Time: " << fixed << setprecision(6) << seconds << " s\n";

    //parallel
    auto start = high_resolution_clock::now();
    pthread_t th[thread_num];
    int thread_id[thread_num];
    chunk_size=n/thread_num;
    rem= n%thread_num;

    for(int k=0;k<n;k++){
        current_k=k;

        for (int i=0;i<thread_num;i++){
            thread_id[i]=i;
            pthread_create(&th[i],NULL,floydWarshallParallel,&thread_id[i]);
        }  
        for (int j = 0; j < thread_num; j++) {
            pthread_join(th[j], NULL);
        }     
        
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    //cout<<"\n parallel after \n";
    //printMatrix(dist_parallel);
    double seconds_par= duration.count() / 1'000'000.0;
    cout << "Parallel Floyd-Warshall Time: " << fixed << setprecision(6) << seconds_par<< " s\n";
    speed=static_cast<float>(duration_seq.count())/duration.count();
    cout<<"speedup: "<<speed<<"x"<<endl;
    if(dist_parallel==dist_seq){ //to check if the results are correct
        cout<<"correct \n";
    }
    else 
    cout<<"incorrect\n";

}
