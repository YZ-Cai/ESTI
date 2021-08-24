#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <set>
#include <map>
#include <queue>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <sstream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
using namespace std;


#define maxn 30000000
#define maxm 60000000


// generate random numbers for windows
#if (defined _WIN32) || (defined _WIN64)
    #define lrand48() ((rand()<<16) | rand())
    #define srand48(n) srand(n)
#else
    #define INT_MAX 2147483648
#endif



// graph structure
int N = 0, M = 0, K = -1, rootN = 0, leafN = 0;
int edge[maxm], ee[maxn], eb[maxn];
int redge[maxm], ree[maxn], reb[maxn];



// obtain running time
struct timeval after_time, before_time;
void startTime() {
	gettimeofday(&before_time, NULL);
}
double endTime() {
	gettimeofday(&after_time, NULL);
    double totaltime = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
        (after_time.tv_usec - before_time.tv_usec)*1.0 / 1000.0;
    
    return totaltime;
}



// load input graph file
vector<pair<int, int>> tmpLoad;
void load(char* fileName) {
    freopen(fileName, "r", stdin);
    set<pair<int, int>> S;
    
    scanf("%d%d", &N, &M);
    M = 0;
    if (N >= maxn) {
        printf("Number of vertices exceeds the limit.\n Try increase the limit.\n");
        exit(-1);
    }
    memset(ee, -1, sizeof ee);
    memset(eb, -1, sizeof eb);
    tmpLoad.clear();
    while (1) {
        int id, nei;
        if (scanf("%d", &id) == EOF) {
            break;
        }
        int deg;
        scanf("%d", &deg);
        if (deg==0)
            leafN++;
        while (deg--) {
            scanf("%d", &nei);
            
            edge[M] = nei;
            if (eb[id] == -1) {
                eb[id] = M;
            }
            tmpLoad.push_back(make_pair(nei, id));
            ee[id] = M + 1;
            M++;
        }
    }
    memset(ree, -1, sizeof ree);
    memset(reb, -1, sizeof reb);
    rootN = N;
    sort(tmpLoad.begin(), tmpLoad.end());
    for(int i=0; i<M; i++) {
        redge[i] = tmpLoad[i].second;
        int& start = tmpLoad[i].first;
        if (reb[start] == -1) {
            reb[start] = i;
            rootN--;
        }
        ree[start] = i + 1;
    }
    printf("N: %d\tM: %d\trootN: %d\tleafN: %d\n", N, M, rootN, leafN);
}



// load input query file
char *queryFile = NULL, *graphFile = NULL;
int queryNumber = -1;
vector<int> qlist[4];
static void loadQueries() {
    if (queryFile){
        int t[4];
        FILE* f = fopen(queryFile, "r");
        while(fscanf(f, "%d%d%d%d", t, t + 1, t + 2, t+3) != EOF) {
            for (int i=0; i<4; i++) {
                qlist[i].push_back(t[i]);
            }
        }
        queryNumber = qlist[0].size();
    } else {
        srand48(time(NULL));
        for (int i=0; i<queryNumber; i++) {
            qlist[0].push_back(lrand48() % N);
            qlist[1].push_back(lrand48() % N);
            qlist[2].push_back(-2);
        }
    }
}



// get current time for writing logs
string current_time( )
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return  to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " +
            to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
}






