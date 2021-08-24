/*
K-hop Reachability - accelerate FELINE index
Author: Yuzheng Cai
2021-1-6
------------------------------
ISO C++ 11 
*/ 

#include "data_structure.h"
using namespace std;


// FELINE index - X coordinate
void FELINE_X(){

    int u;
    order = 0;
    stk_end = 0;
    
    // find those don't have in-edges
    for (int i=0; i<root_size; i++) {
        stk[stk_end] = roots[i];
        stk_end++;
    }

    // the stack isn't empty
    while (stk_end) {   
        stk_end--;   
        u = stk[stk_end];                                   // pop current vertex u
        DAG_X[u] = order;
        for (int i=DAG2raw_b[u]; i<DAG2raw_e[u]; i++)       // X order of current DAG vertex u's original vertices
            ESTI[DAG2raw[i]].X = order;
        order++;  
        
        for (int i=DAG2raw_b[u]; i<DAG2raw_e[u]; i++) {
            int& v = DAG2raw[i];                            // j th old id v which is contained in new id i
            for (int k=new_eb[v]; k<new_ee[v]; k++) {       // all out-edges of v into i's out-edges
                int& out_nei = raw2DAG[new_edge[k]];
                if (out_nei!=u) {                           // if the edge doesn't point to i itself
                    tmp_in_edges[out_nei]--;                // delete edges connect current vertex v to other unvisited vertexes
                    if (tmp_in_edges[out_nei]==0) {
                        stk[stk_end] = out_nei;
                        stk_end++;
                    }
                }
            }
        }
    }
}


// FELINE index - Y coordinate
void FELINE_Y(){
    
    int u;
    order = 0;

    // add those don't have in-edges, note that they're already in descending X order when pop from stack
    for (int i=root_size-1; i>=0; i--){
        stk[stk_end] = roots[i];
        stk_end++;
    }
    
    // the stack isn't empty
    while (stk_end) {      
        stk_end--;
        u = stk[stk_end];                                   // pop current vertex u
        for (int i=DAG2raw_b[u]; i<DAG2raw_e[u]; i++)       // Y order of current DAG vertex u's original vertices
            ESTI[DAG2raw[i]].Y = order;
        order++;  
        processed_vn = stk_end;

        for (int i=DAG2raw_b[u]; i<DAG2raw_e[u]; i++) {
            int& v = DAG2raw[i];                            // j th old id v which is contained in new id i
            for (int k=new_eb[v]; k<new_ee[v]; k++) {       // all out-edges of v into i's out-edges
                int& out_nei = raw2DAG[new_edge[k]];
                if (out_nei!=u) {                           // if the edge doesn't point to i itself
                    tmp_in_edges[out_nei]--;                // delete edges connect current vertex v to other unvisited vertexes
                    if (tmp_in_edges[out_nei]==0) {
                        stk[stk_end] = out_nei;
                        stk_end++;
                    }
                }
            }
        }

        sort(stk+processed_vn, stk+stk_end, comp_X);        // sort the vertex by X coordinate
    }
}




// FELINE index - X coordinate (only works for the input graph is DAG)
void FELINE_X_DAG(){

    int u;
    order = 0;
    stk_end = 0;
    
    // find those don't have in-edges
    for (int i=0; i<rootN; i++) {
        stk[stk_end] = roots[i];
        stk_end++;
    }

    // the stack isn't empty
    while (stk_end) { 
        stk_end--;   
        u = stk[stk_end];                                   // pop current vertex u
        DAG_X[u] = order;
        ESTI[u].X = order;
        order++;  
        
        for (int j=new_ee[u]-1; j>=new_eb[u]; j--){         // delete edges connect current vertex v to other unvisited vertexes
            int& v = new_edge[j];                           // an out-edge from u to v
            tmp_in_edges[v]--;
            if (tmp_in_edges[v] == 0) {                     // if all of vertex v's in-edges have been deleted, push v into the stack
                stk[stk_end] = v;
                stk_end++;
            }  
        }
    }
}


// FELINE index - Y coordinate (only works for the input graph is DAG)
void FELINE_Y_DAG(){
    
    int u;
    order = 0;
    stk_end = 0;
    
    // add those don't have in-edges, note that they're already in descending X order when pop from stack
    for (int i=rootN-1; i>=0; i--){
        stk[stk_end] = roots[i];
        stk_end++;
    }
    
    // the stack isn't empty
    while (stk_end) {      
        stk_end--;
        u = stk[stk_end];                                   // pop current vertex u
        ESTI[u].Y = order;
        order++; 
        processed_vn = stk_end;

        for (int j=new_eb[u]; j<new_ee[u]; j++){            // delete edges connect current vertex v to other unvisited vertexes
            int& v = new_edge[j];                           // an out-edge from u to v
            tmp_in_edges[v]--;
            if (tmp_in_edges[v] == 0) {                     // if all of vertex v's in-edges have been deleted, push v into the stack
                stk[stk_end] = v;
                stk_end++;
            }  
        }

        sort(stk+processed_vn, stk+stk_end, comp_X);        // sort the vertex by X coordinate
    }
}


void accelerateFELINE(){
    
    stk_end = 0;
    
    if (is_DAG) {

        // FELINE index - X coordinate
        memcpy(tmp_in_edges, DAG_in_degree, sizeof(int)*DAG_vn);        
        FELINE_X_DAG();

        // FELINE index - Y coordinate
        memcpy(tmp_in_edges, DAG_in_degree, sizeof(int)*DAG_vn);
        FELINE_Y_DAG();

    } else {

        // finding roots in DAG
        for (int i=DAG_vn-1; i>=0; i--)
            if (DAG_in_degree[i]==0) {
                roots[stk_end] = i;
                stk_end++;
            }
        root_size = stk_end; 

        // FELINE index - X coordinate
        memcpy(tmp_in_edges, DAG_in_degree, sizeof(int)*DAG_vn);        
        FELINE_X();

        // FELINE index - Y coordinate
        memcpy(tmp_in_edges, DAG_in_degree, sizeof(int)*DAG_vn);
        FELINE_Y();
    
    }
        
}



