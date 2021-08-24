#define maxn 26000000
#define maxm 100000000
#define query_num 1000
#include "../common.h"
#define max_query_num 100000000

vector<int> query[maxn];
vector<pair<pair<int, int>, int> > Qlist;
int V[maxn];
queue<int> Q;
vector<int> hop_distribution;


void printUsageofGenQueries() {
    puts("Usage: generate queries -data graph -outputQueryFile [ -queryNumber ] ");
    puts("\tgraph: input graph file name");
    puts("\toutputQueryFile: output query file");
    puts("\tQueryNumber: the number of randomly generated queries");
}


int main(int argc, char* argv[]){
	
    if (argc<3) {
        printUsageofGenQueries();
        exit(-1);
    }
    
    int K;
    if (argc==3)
        K = query_num;
    else
        K = atoi(argv[3]);
        
    cout<<"\nGenerate "<<K<<" queries for "<<argv[1]<<endl; 
    load(argv[1]);
    
    for (int i=0; i<K; i++) {
        int q1 = lrand48() % N;
        int q2 = lrand48() % N;
        query[q1].push_back(q2);
    }
    {
        for (int i=0; i<N; i++) {
            if (M>100000 && i%(N/100)==0)
                cout<<"Generated "<<Qlist.size()<<" queries"<<endl;
                
            if (!query[i].size()) {
                continue;
            }
            memset(V, -1, sizeof V);
            int count = 1;
            V[i] = 0;
            while (!Q.empty()) {
                Q.pop();
            }
            Q.push(i);
            while (!Q.empty()) {
                int cur = Q.front();
                Q.pop();
                for(int j=eb[cur]; j<ee[cur]; j++) {
                    int nxt = edge[j];
                    if (V[nxt] == -1) {
                        V[nxt] = V[cur] + 1;
                        Q.push(nxt);
                        count++;
                    }
                }
            }
            for (int j=0; j<query[i].size(); j++) {
                int nxt = query[i][j];
                Qlist.push_back(make_pair(make_pair(i, nxt), V[nxt]));
                if (V[nxt]>0) 
                    hop_distribution.push_back(V[nxt]);
            }
        }
    }
    int S = Qlist.size();
    for (int tmp=0; tmp<Qlist.size(); tmp++) {
        int t2 = (long long)lrand48() * lrand48() % S;
        swap(Qlist[tmp], Qlist[t2]);
    }

    if (hop_distribution.size()==0) {
            cerr<<"They're no reachable pairs in " << argv[1] << endl;
            cout<<endl;
            exit(-1);
        }

    // write into a query file
    std::ofstream query_file(argv[2]);
    if (!query_file) {
        cerr << "error in creating file" << argv[2] << endl;
    } else {
        for (int i=0; i<Qlist.size(); i++) {
            int hop_num = hop_distribution[lrand48()%hop_distribution.size()];
            query_file<<Qlist[i].first.first<<"\t"<<Qlist[i].first.second<<"\t"<<Qlist[i].second<<"\t"<<hop_num<<std::endl;
        }
        query_file.close();
        cout<<"Successfully complete! output file:\t"<<argv[2]<<endl;
    }
    
    return 0;
}

