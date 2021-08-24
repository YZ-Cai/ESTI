/*
K-hop Reachability - Condense SCCs in Reduced Graph to Obtain DAG
Author: Yuzheng Cai
2020-1-6
------------------------------
ISO C++ 11 
------------------------------
Notice:
1. A graph with cycles is turned into a DAG by condensing Strongly Connected Components(SCC);
2. The algorithm used here is Tarjan Algorithm;
*/ 

#include "data_structure.h"
using namespace std;


// tarjan algorithm to find SCCs
void tarjan(int u){

    DFN[u]=LOW[u]=order;                            // DFN and LOW value by traversal order
    order++;
    visited[u] = true;
    stk[stk_end] = u;
    stk_end++;
    instack[u] = true;

    for (int i=eb[u]; i<ee[u]; i++){   
        int& v = edge[i];
        if (visited[v]==false){                     // hasn't been visited
            tarjan(v);
            LOW[u] = LOW[u]<LOW[v]?LOW[u]:LOW[v];   // LOW should be the smallest among its childs
        } else if (instack[v]){                     // find a cycle         
            LOW[u] = LOW[u]<DFN[v]?LOW[u]:DFN[v];   // update LOW
        }
    }
    
    int t;
    if (DFN[u]==LOW[u]){                            // a new SCC 
        DAG2raw_b[new_id] = processed_vn;
        do {
            stk_end--;
            t = stk[stk_end];                       // old id belong to new id
            DAG2raw[processed_vn] = t;              
            processed_vn++;                         
            instack[t] = false;
            raw2DAG[t] = new_id;                    // from old id to new id
        } while (t!=u);
        DAG2raw_e[new_id] = processed_vn;
        new_id++;                                   // next new id
    }
}


// convert a graph with cycles into a DAG
void generate_DAG(){

    instack = new bool[N]();                        // remember which vertex is in stack
    DFN = new int[N];                               // vectors used in Tarjan Algorithm
    LOW = new int[N];                               // vectors used in Tarjan Algorithm            
    raw2DAG = new int[N];                           // matches from old id to new id in DAG
    processed_vn = 0;
    DAG2raw = new int[N];                           // matches from new id in DAG to old id
    DAG2raw_b = new int[N];
    DAG2raw_e = new int[N];

    // run Tarjan to find SCCs
    order = 0;
    new_id = 0;
    for (int i=0; i<N; i++)
        if (visited[i]==false) 
            tarjan(i);
    DAG_vn = new_id;

    if (is_DAG==false) {
        DAG_in_degree = new int[DAG_vn]();                  // in-degree of vertices in DAG
        for (int i=0; i<DAG_vn; i++){                       // for each new id
            for (int j=DAG2raw_b[i]; j<DAG2raw_e[i]; j++){  // for each old id which is contained in new id i
                int& v = DAG2raw[j];                        // j th old id v which is contained in new id i
                for (int k=new_eb[v]; k<new_ee[v]; k++)     // add all out-edges of v into i's out-edges
                    if (raw2DAG[new_edge[k]]!=i)            // if the edge doesn't point to i itself
                        DAG_in_degree[raw2DAG[new_edge[k]]]++;
            }
        }
    }
    cout<<"Its DAG has totally "<<DAG_vn<<" vertices"<<endl;
}