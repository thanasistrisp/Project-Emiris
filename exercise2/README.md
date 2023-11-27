# Software Development for Algorithmic Problems - Winter Semester 2023-24

# Project 2 - Graph Nearest Neighbour Search in C/C++

Eleftheria Vrachoriti - 1115202000026

Athanasios Trispiotis - 1115202000194

# Table of Contents
- [1. Project structure](#1-project-structure)
- [2. Compilation](#2-compilation)
  - [2.1. Main program `graphsearch`](#21-main-program-graphsearch)
  - [2.2. Shared library `graphsearch.so`](#22-shared-library-graphsearchso)
- [2.3. clean](#23-clean)
- [3. Execution](#3-execution)
  - [3.1. Main program `graphsearch`](#31-main-program-graphsearch)
  - [3.2. Testing - Hyperparameter tuning](#32-testing---hyperparameter-tuning)
    - [3.2.1. Execution](#321-execution)
    - [3.2.2. Analysis](#322-analysis)
- [4. Documentation](#4-documentation)
  - [4.1. Directed Graph](#41-directed-graph)
  - [4.2. Graph Nearest Neighbor Search](#42-graph-nearest-neighbor-search)
    - [4.2.1. Construction](#421-construction)
    - [4.2.2. GNNS Query Search Algorithm](#422-gnns-query-search-algorithm)
  - [4.3. Monotonic Relative Neighborhood Graph](#43-monotonic-relative-neighborhood-graph)
    - [4.3.1. Construction](#431-construction)
    - [4.3.2. GNNS Query Search Algorithm](#432-mrng-query-search-algorithm)
  - [4.4 General Implementation Details](#44-general-implementation-details)
- [References](#references)


# 1. Project structure
```bash
exercise2/
├── source_code				    # directory for source code of the main program
│   ├── common
│   │   ├── directed_graph.cc	    # implementation of the directed graph
│   │   ├── generic_search.cc	    # implementation of the generic search on graph (used for mrng)
│   │   └── handle_output.cc	    # helper function for processing the output
│   │
│   ├── gnn						# directory for source files for GNNS (construction, query) implementation
│   │   └── gnn.cc
│   │
│   ├── mrng					# directory for source files for MRNG (construction) implementation
│   │   └── mrng.cc
│   │
│   └── main.cc
│
├── include						# directory for header files
│   ├── directed_graph.hpp
│   ├── generic_search.hpp
│   ├── gnn.hpp
│   ├── handling.hpp
│   ├── mrng.hpp
│   └── set_utils.hpp			    # overloading of the set operators for using set of pairs
│
├── testing						# directory for testing files (hyperparameter tuning)
│   ├── optimization.ipynb	        # jupyter notebook for hyperparameter tuning including plots
│   ├── params.py			        # functions for handling ctypes (lsh, cube, gnn, mrng)
│   └── python_connector.cc	        # exposing the C++ functions to python
│
├── lib							# directory for shared library
│   ├── Makefile				    # makefile for building only the shared library
│   └── graphsearch.so
│
├── MNIST					    # directory for input and query data files
│   ├── input.dat
│   └── query.dat
│
├── graph_files				    # directory for saved graph files
│   ├── gnn_graph.bin
│   └── mrng_graph.bin
│
├── output					    # directory for result files
│   └── output.txt
│
├── common.mk				    # main makefile
├── Makefile				    # makefile for building the main program (including the shared library)
├── README.md				    # this documentation file
│
├── ../exercise1/			    # directory for source code of the first project
│   └── ...
```

# 2. Compilation

## 2.1. Main program `graphsearch`

Stay in the root directory <code>exercise2/</code> and then run the following command:

```bash
make
```

This will build the main program <code>graphsearch</code> and the shared library <code>graphsearch.so</code> in the <code>lib/</code> directory.

## 2.2. Shared library `graphsearch.so`

Stay in the root directory of the <code>exercise2/</code> and then run the following command:

```bash
cd lib
make
```

This will build only the shared library <code>graphsearch.so</code> in the <code>lib/</code> directory.

# 2.3. clean

To remove dependency, object, shared library and executable files, run the following command:

```bash
make clean
```

at any of the two following directories:

+ `.`
+ `lib`

# 3. Execution

## 3.1. Main program `graphsearch`

After running the commands in [2.1.](#21-main-program-graphsearch), run the following command at the root directory of the <code>exercise2/</code>:

```bash
./graphsearch -d <input file> -q <query file> -k <int> -E <int> -R <int> -N <int> -l <int, only for Search-on-Graph> -m <1 for GNNS, 2 for MRNG> -o <output file> -save <save graph file> -load <load graph file>
```

where:

+ `input file`: binary input data in the form that's specified in [[1]](#references)
+ `query file`: binary query data in the form that's specified in [[1]](#references)
+ `k`: number of nearest neighbors in k-NN graph
+ `E`: number of expansions
+ `R`: number of random restarts
+ `N`: number of Approximate Nearest Neighbours returned
+ `l`: number of candidate pool size (only for Search-on-Graph)
+ `m`: 1 for GNNS, 2 for MRNG
+ `output file`: file for output
+ `save graph file`: binary file for saving the graph (optional)
+ `load graph file`: binary file for loading the graph (optional)

If any of the numeric arguments aren't specified except for `m`, the following values will be used:

| Argument | Default value |
|:------:|:------:|
| `k` | 50 |
| `E` | 30 |
| `R` | 1 |
| `N` | 1 |
| `l` | 20 |

e.g.

```bash
./graphsearch -d MNIST/input.dat -q MNIST/query.dat -k 50 -E 30 -R 1 -N 1 -l 20 -m 1 -o output/output.txt
```

Alternatively, you can skip [2.1.](#21-main-program-graphsearch) and compile and execute `graphsearch` with our chosen arguments using only the following commands:

```bash
make run
```

To check memory leaks with `valgrind`, use the following command:

```bash
make valgrind
```

## 3.2. Testing - Hyperparameter tuning

### 3.2.1. Execution

To perform hyperparameter analysis, you need to have `conda` installed. Then, you can create a conda environment by running the following command:

```bash
conda env create -f environment.yml # environment.yml exists in testing directory
```

You can open now the `optimization.ipynb` jupyter notebook in <code>testing</code> directory and run the cells.

To bridge c++ and python, we used `ctypes` [[4]](#references) and we created the `params.py` file which contains the functions for handling the ctypes for each algorithm. The `python_connector.cc` file exposes the C++ functions to python by returning primitive C types (c_int, c_char_p, c_POINTER, etc)
using `extern "C"`, a linkage specification.

### 3.2.2. Analysis

Analysis is done in the corresponding jupyter notebook. The notebook contains the hyperparameter tuning for the following algorithms:

+ GNNS
+ MRNG
+ LSH
+ Cube

The results are already run and saved inside the notebook.

# 4. Documentation

Note: All times and parameters referenced below are optimized for the full MNIST dataset of 60000 images.

## 4.1. Directed Graph

Basic directed graph module used for the implementation of the GNNS and MRNG algorithms.

The graph is implemented using adjacency lists. More specifically, a vector of vectors is used `std::vector<std::vector<int>>`. The indices of the first vector are the indices of the graph's nodes and each sub-vector acts as an adjacency list that stores the indices of each node's successors.

The module provides all basic functions to add nodes and successors and to retrieve a node's successors, as well as predecessors.

In addition to that, a set of save/load functions have been implemented to save/load the graph into a `.bin` file. Two examples can be found in the `graph_files/` directory. This feature has been really useful for debugging the MRNG query process, because the MRNG construction is too time-consuming to be used from scatch every time.

## 4.2. Graph Nearest Neighbor Search (GNNS)

### 4.2.1. Construction

LSH is used to construct the directed graph, using the following parameters derived from the hyperparameter tuning using the constraints:

neighbors returned $\geq 50$ and average query time $\leq 0.01$ sec while optimizing $\min$ {MAF}, $\min$ {average query time}:

| LSH |
|:------:|
| $k = 7$ |
| $L = 4$ |
| $table\_size = 15000$ |
| $w = 1815$ |
| $query = false$ |

For each point $p$ in the dataset, we retrieve its $k$-nearest neighbors by using the LSH algorithm and we add an edge between $p$ and each of its neighbors.

**NOTE**:

Even though most of the times, the LSH algorithm returns at least $k=50$ neighbors, as the algorithm is probabilistic, we have no guarantee that it will always return such a number of neighbors. 

For that reason, we have employed two simple techniques that guarantee the existence of at least $k$ neighbors; first we try to use the successors of the node's predecessor as neighbors, trying to avoid the large distances that random neighbors would have. However, the problem might persist, so eventually, random neighbors will have to be added as well.

Average construction time: 50-70 seconds.

### 4.2.2. GNNS Query Search Algorithm

The GNNS Algorithm Query Search Algorithm is the best-first search algorithm presented in the course slides.

In comparison to the pseudo-code given, no greedy moves $T$ are used, but it is checked if a node reached is better than its new neighbors (local optima).
We also stop the search if we return to a node that we have already visited (i.e. cyclic path in graph). There is no need to sort distances at the end, as we use
an ordered set.

## 4.3. Monotonic Relative Neighborhood Graph

### 4.3.1. Construction

LSH is used to construct the directed graph, using we the following parameters derived from the hyperparameter tuning using the constraints:

neighbors returned $\geq 50$ and average query time $\leq 0.01$ sec while optimizing $\min$ {MAF}, $\min$ {average query time}:

| LSH |
|:------:|
| $k = 7$ |
| $L = 4$ |
| $table\_size = 15000$ |
| $w = 1815$ |
| $query = false$ |

The construction algorithm follows the steps described in [[3]](#references). For each point $p$ in the dataset, we construct a set $R_p$ containing all points except for $p$. The next step is to find all neighbors that share the same minimum distance from $p$ and add them to a new set, $L_p$. 

**NOTE**:

Even though the LSH algorithm almost always succeeds at finding one neighbor for each query, there are still cases in which it may fail, because the algorithm is probabilistic. For that reason, we have employed an naive technique that guarantees the existence of at least one neighbor for each point; the addition of a random neighbor.

Average construction time: 8 hours.

### 4.3.2. MRNG Query Search Algorithm

The algorithm used for querying the MRNG graph is the Search-on-Graph (Generic Search) algorithm proposed presented in the course slides.

This algorithm has been implemented in a different file, because it may be applied to other types of graphs as well, and not just to MRNG graphs.

## 4.4 General Implementation Details

For the implementation of the algorithms with sets in most cases, we used the following data structures:

+ `std::[unordered_]multiset<pair<double,int>>` for storing the distances and the indices of the points (multisets are used because we could have duplicates in the distance key for different points).

    For ordered sets, we have defined a comparator function.
    
    For unordered sets, we have defined a hash and an equality function in `set_utils.hpp`.
+ `std::unordered_set<int>` for storing the indices of the points as the multiset data structure does not check for duplicate pairs or if a point has been visited before.

# References

[1] LeCun, Y., Cortes, C., & Burges, C.. THE MNIST DATABASE
of handwritten digits. https://yann.lecun.com/exdb/mnist/

[2] Hajebi, K., Abbasi-Yadkori, Y., Shahbazi, H. & Zhang, H. (2011). Fast Approximate Nearest-Neighbor Search with k-Nearest Neighbor Graph. 1312-1317. https://doi.org/10.5591/978-1-57735-516-8/IJCAI11-222

[3] Fu, C., Xiang, C., Wang, C., & Cai, D. (2019). Fast approximate nearest neighbor search with the navigating spreading-out graph. Proceedings of the VLDB Endowment, 12(5), 461–474. https://doi.org/10.14778/3303753.3303754 

[4] ctypes — A foreign function library for Python
https://docs.python.org/3/library/ctypes.html

[5] Optuna - A hyperparameter optimization framework
https://optuna.org/