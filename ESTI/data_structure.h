/*
K-hop Reachability - Data Structure
Author: Yuzheng Cai
2021-1-6
------------------------------
ISO C++ 11 
*/ 

#ifndef  DATASTRUCTURE_H
#define  DATASTRUCTURE_H

#include "../common.h"
using namespace std; 


struct EST_node{						// extended spanning tree node
	int level;
	int pre;
	int post;
	int virtual_dist;
	EST_node** children;
	int children_cnt;
};

struct ESTI_index{						// extended spanning tree index
	EST_node* real_and_virtual_nodes;
	int X, Y;
};


/*
shared variables
*/
bool reversed=false;                    // whether the graph has been reversed
bool is_DAG=false;                      // the input graph is DAG
bool visited[maxn];						// mark those visited vertices
int order;							    // current order for DFS traversal
int stk[maxn];                          // stack for DFS / frontier for BFS
int stk_end;
int processed_vn;
int *new_edge, *new_redge, *new_reb, *new_ree, *new_eb, *new_ee; 


#define in_degree(x) (new_ree[(x)]-new_reb[(x)] + ((link2root[(x)>>5]&(1<<((x)&31)))!=0))
#define link(x) link2root[(x) >> 5] |= 1 << ((x) & 31)


/*
DAG generation
*/
int new_id;                             // new vertex id after condensing SCC
bool* instack;                          // remember which vertex is in stack
int *DFN, *LOW;                         // vectors used in Tarjan Algorithm
int DAG_vn;
int* raw2DAG;                           // match raw vertex id to new vertex id 
int *DAG2raw_b, *DAG2raw_e, *DAG2raw;   // match new vertex id to raw vertex id
int* DAG_in_degree;					    // in-degree of vertices in DAG
#ifdef TIME_DETAIL
	double genDAG_time;
#endif



/*
accelerated FELINE index generation
*/
int roots[maxn];						// roots in DAG
int root_size;
int tmp_in_edges[maxn];      			// record vertexes' in-degree in traversal
int DAG_X[maxn];						// record X order for vertices in DAG
bool comp_X(int a, int b){				// sort the vertex by X coordinate
	return DAG_X[a]<DAG_X[b];
} 

/*
ESTI index generation
*/
int q_begin, q_end;
int* EST_cnt;						    // count that how many nodes have been indexed


/*
final index
*/
int link2root[maxn/32+1];				// whether a vertex is link to virtual root
ESTI_index* ESTI;					    // ESTI index storage


/*
online query
*/
struct tri{
	int s;
	int t;
	int k;
};
tri* tri_stk;
bool comp_pre(EST_node a, EST_node b){				// sort the real and virtual nodes by pre coordinate
    return a.pre<b.pre;
};


/*
calculate index size (in bytes)
*/
long long get_index_size(){
	long long link2root_len = N/32+1;
	long long ESTI_len = 0;
	for (int i=0; i<N; i++) {
		ESTI_len += 2;							    // int X, Y
		ESTI_len += 4;							    // virtual nodes (int level,pre,post,virtual_dist)
		ESTI_len += (long long) (in_degree(i)-1)*2; // virtual nodes (int level,pre)
	}
	return (link2root_len+ESTI_len)*sizeof(int)+sizeof(bool);
}



#endif 