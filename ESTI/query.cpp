/*
K-hop Reachability - ESTI querying function
Author: Yuzheng Cai 
2021-1-10
------------------------------
ISO C++ 11 
*/ 

#include "data_structure.h"
using namespace std; 



// query function with prune 1, 2 and 3
bool ESTI_query(int start, int end, int query_k){

	if (start==end)
		return true;
	tri_stk[0] = {start, end, query_k};
	stk_end = 0;

	while (stk_end>=0) {
		
		int u = tri_stk[stk_end].s;
		int v = tri_stk[stk_end].t;
		int k = tri_stk[stk_end].k;
		stk_end--;	
		
		if (ESTI[u].X>ESTI[v].X || ESTI[u].Y>ESTI[v].Y || 
			ESTI[v].real_and_virtual_nodes[0].level - ESTI[u].real_and_virtual_nodes[0].level > k)		// Prune 1 
			continue;	

		if (ESTI[v].real_and_virtual_nodes[0].pre  >  ESTI[u].real_and_virtual_nodes[0].pre &&
			ESTI[v].real_and_virtual_nodes[0].post <  ESTI[u].real_and_virtual_nodes[0].post &&
			ESTI[v].real_and_virtual_nodes[0].level-ESTI[u].real_and_virtual_nodes[0].level<=k)
			return true;	

		if (in_degree(v)-1) {
			EST_node* v_start = lower_bound(	ESTI[v].real_and_virtual_nodes+1, 						// Prune 3
												ESTI[v].real_and_virtual_nodes+in_degree(v), 
												ESTI[u].real_and_virtual_nodes[0],
												comp_pre);
			while (	v_start != ESTI[v].real_and_virtual_nodes+in_degree(v) &&
					v_start->pre < ESTI[u].real_and_virtual_nodes[0].post ) {
						if (v_start->level - ESTI[u].real_and_virtual_nodes[0].level <= k) 
							return true;
						v_start++;
					}
		}

		if (k>1 && ESTI[u].real_and_virtual_nodes[0].virtual_dist < k)									// Prune 2
			if (new_ee[u]-new_eb[u]<=new_ree[v]-new_reb[v]) 
				for (int i=new_eb[u]; i<new_ee[u]; i++){
					stk_end++;
					tri_stk[stk_end] = {new_edge[i], v, k-1};
				}
			else	
				for (int i=new_reb[v]; i<new_ree[v]; i++){
					stk_end++;
					tri_stk[stk_end] = {u, new_redge[i], k-1};
				}
	}
	
	return false;
}