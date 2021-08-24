/*
K-hop Reachability - ESTI indexing and query
Author: Yuzheng Cai 
2021-1-6
------------------------------
ISO C++ 11 
*/ 


#include "data_structure.h"
#include "generate_DAG.cpp"
#include "accelerateFELINE.cpp"
#include "query.cpp"
using namespace std; 


void printUsageofESTI() {
    puts("Usage: ESTI graphFile queryFile [DAG/cyc]");
    puts("\tgraphFile: the graph file name");
    puts("\tqueryFile: the query file");
	puts("\tDAG/cyc: whether the input graph is DAG or cyclic");
}


// perform DFS for linking to virtual root
void explore_for_virtual_root(int i){
	new_edge[M] = i;
	M++;
	stk[0] = i;
	stk_end = 1;
	visited[i] = true;
	while (stk_end) {
		stk_end--;
		int v = stk[stk_end];
		for(int j=new_eb[v]; j<new_ee[v]; j++ )
			if (visited[new_edge[j]]==false) {
				visited[new_edge[j]] = true;
				stk[stk_end] = new_edge[j];
				stk_end++;
			}
	}
}


// perform DFS for pre- and postorder index, and also update virtual_dist index (for Prune 2)
void pre_post_index(EST_node* cur){
	cur->pre = order;
	order++;
	for (int i=0; i<cur->children_cnt; i++) {
		pre_post_index(cur->children[i]);
		cur->virtual_dist = min(cur->virtual_dist, cur->children[i]->virtual_dist+1);
	}
	cur->post = order;
	order++;
}



int main(int argc, char* argv[]){
	
	/*
	load in graph data
	*/
	if (argc>3) {
		graphFile = argv[1];
		queryFile = argv[2];
		if (strcmp(argv[3], "DAG")==0)
			is_DAG = true;
	} else 
		printUsageofESTI();
	
	printf("\n--------------- ESTI ----------------\n");
	printf("Filename: %s\n", graphFile);
	printf("is_DAG:%d\n",is_DAG);
	load(graphFile);

	
	
	/*
	start recording time cost
	*/
	startTime();



	/*
	whether to reverse the graph
	*/
	if (rootN>2*leafN){
		reversed = true;
		new_reb = eb;
		new_eb = reb;
		new_ee = ree;
		new_ree = ee;
		new_edge = redge;
		new_redge = edge;
		root_size = leafN;
		leafN = rootN;
		rootN = root_size;
		printf("the graph has been reversed\n");
	} else {
		new_eb = eb;
		new_reb = reb;
		new_ee = ee;
		new_ree = ree;
		new_edge = edge;
		new_redge = redge;
	}



	/*
	initialization
	*/
	ESTI = new ESTI_index[N+1];
	


	/*
	link to the virtual root node
	*/
	new_eb[N] = M;
	if (is_DAG) {
		stk_end = 0;
		DAG_in_degree = new int[N];
		for (int i=0; i<N; i++) {
			if (new_reb[i]==-1) { 					// no in-neighbor
				new_edge[M] = i;
				M++;
				roots[stk_end] = i;
				stk_end++;
				link(i);
			}
			DAG_in_degree[i] = new_ree[i]-new_reb[i];
		}  
	} else {
		for (int i=0; i<N; i++) 
			if (new_reb[i]==-1)	{					// no in-neighbor
				explore_for_virtual_root(i); 
				link(i);
			}      
		for (int i=0; i<N; i++)
			if (visited[i]==false) {					// haven't been visited
				explore_for_virtual_root(i);
				link(i);
			}
	}
	new_ee[N] = M;



	/*
	Accelerate FELINE index generation
	*/		
	memset(visited, 0, sizeof(bool)*N);             // mark which vertex has been visited
	if (is_DAG)
		DAG_vn = N;
	else
		generate_DAG();
	accelerateFELINE();
	
	
	
	/*
	establish the extended spanning tree and constrcut ESTI index
	*/
	// initialize
	EST_cnt = new int[N+1]();							// count that how many nodes have been indexed
	
	// BFS construction and level index assignment
	ESTI[N].real_and_virtual_nodes = new EST_node[1];
	ESTI[N].real_and_virtual_nodes[0].level = 0;
	q_begin = 0;
	q_end = 1;
	stk[0] = N;										// virtual root's id=N, level=0
	while (q_begin<q_end) {
		
		// current vertex to explore
		int& cur = stk[q_begin];
		int& level = ESTI[cur].real_and_virtual_nodes[0].level;
		q_begin++;

		// explore cur's neighbors
		if (new_eb[cur]!=-1) {
			ESTI[cur].real_and_virtual_nodes[0].children = new EST_node*[new_ee[cur]-new_eb[cur]];
			ESTI[cur].real_and_virtual_nodes[0].children_cnt = new_ee[cur]-new_eb[cur];
			for (int i=new_eb[cur]; i<new_ee[cur]; i++) {
				int& nei = new_edge[i];

				if (EST_cnt[nei]==0) {					// nei hasn't been explored, create a real node
					ESTI[nei].real_and_virtual_nodes = new EST_node[in_degree(nei)];
					ESTI[nei].real_and_virtual_nodes[0].virtual_dist = N+1;;
					stk[q_end] = nei;					// only real nodes need to be explored	
					q_end++;
				} else {								// nei has been explored, create a virtual node
					ESTI[nei].real_and_virtual_nodes[EST_cnt[nei]].virtual_dist=0;		
					ESTI[nei].real_and_virtual_nodes[EST_cnt[nei]].children_cnt=0;
				}
				
				ESTI[nei].real_and_virtual_nodes[EST_cnt[nei]].level = level+1;
				ESTI[cur].real_and_virtual_nodes[0].children[i-new_eb[cur]] = &(ESTI[nei].real_and_virtual_nodes[EST_cnt[nei]]);
				EST_cnt[nei]++;					
			}
		} else
			ESTI[cur].real_and_virtual_nodes[0].children_cnt = 0;
	}	
		

	
	// perform DFS for pre- and postorder index
	order = 0;
	pre_post_index(&(ESTI[N].real_and_virtual_nodes[0]));

	for (int i=0; i<N; i++)
		if (in_degree(i)-1)
			sort(ESTI[i].real_and_virtual_nodes+1, ESTI[i].real_and_virtual_nodes+in_degree(i), comp_pre);
	


	/*
	finish index construction
	*/
	double index_time = endTime();
	printf("construction time: %.4lf ms\n", index_time);
    printf("index size: %lld bytes\n\n", get_index_size());
	
	// write to results
    ofstream comparison("results/comparison.csv", ios::app);  
    comparison<<"ESTI,,,,,"<<get_index_size()<<","<<index_time<<",";
    


	/* 
	start query test
	*/
	loadQueries();
	bool result;
	double query_time0=-1,query_time1=-1,query_time2=-1,query_time3=-1;
	tri_stk = new tri[maxn];


	/* 
	ESTI query with prune 1+2+3
	*/
	printf("ESTI Querying...\n");
	startTime();
	for(int i=0; i<queryNumber; i++) {
		int& start = qlist[reversed][i];
		int& end = qlist[!reversed][i];
		int& answer = qlist[2][i];
		K = qlist[3][i];

		result = ESTI_query(start, end, K);
		/* if (result ^ ( answer>=0 && answer <= K)) 
			printf("ERROR %d, start: %d, end: %d, answer: %d, result: %d\n", i, start, end, answer, result); */
	}
	double query_time =  endTime();
    printf("query time: %.4lf ms\n", query_time);


	/*
	write to results
	*/
	ofstream details("../results/ESTI_details.csv", ios::app); 
	details<<graphFile<<","<<N<<","<<M<<","<<get_index_size()<<","<<index_time<<","<<query_time<<','<<current_time()<<endl;
	details.close();
	
	return 0;
}
