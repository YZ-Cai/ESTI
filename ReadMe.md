# Source Codes of ESTI



ESTI is an algorithm for answering k-hop  reachability queries over general directed graphs, link of the paper:

[ESTI: Efficient k-Hop Reachability Querying over Large General Directed Graphs | SpringerLink](https://link.springer.com/chapter/10.1007%2F978-3-030-73216-5_6)



## 1. Input Graph

An input graph file example (datasets/sample_graph.txt):

```
7
8
0	2	1	5
1	1	2
2	2	3	4
3	1	1
4	1	0
5	0
6	1	0
```

The first line is the number of vertices (N), while the second line is the number of the edges (M).

The follows N lines are the out-degree and out-neighbors of each vertice. For example, the third line is for vertex "0", and its out-degree is 2, its out-neighbors are "1" and "5".



## 2. Generate Queries

In folder "GenQueries", please run the "make" command to compile.

**Usage:** 

```
generate_queries -data graph -outputQueryFile -queryNumber  
```

graph: 					   input graph file name
outputQueryFile: 	output query file
queryNumber: 		the number of randomly generated queries

**Example:**

```
./generate_queries ../datasets/sample_graph.txt ../datasets/sample_graph.query 10
```

**Query File Format:**

In query file, each line represents a random generated query. 

Each line contains 4 numbers: the first is source vertex, the second is target vertex, the third one is their shortest distance (ground truth for judge the result, -1 for unreachable pair), and the last one is the hop number for query (generated randomly follows the distance distribution of reachable pairs).



## 3. ESTI

In folder "ESTI", please run the "make" command to compile.

**Usage:**

```
ESTI graphFile queryFile [DAG/cyc]
```

graphFile: the graph file name

queryFile: the query filename

DAG/cyc:   whether the input graph is DAG or cyclic

**Example:**

```
./ESTI ../datasets/sample_graph.txt ../sample_graph.query cyc
```

Note that for linux, the size of running stack may need to extend before running the program, for example, set to 2048 MB:

```
ulimit -s 2097152 
```

**Running Results:**

the running results including index time, index size and query time are stored in file "results/ESTI_details.csv".





