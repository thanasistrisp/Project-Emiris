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
  - [4.4 General Implementation Details](#44-general-implementation-details)
- [References](#references)


# 1. Project structure
```bash
exercise2/
├── source_code					# directory containing the source code for the main program
│   ├── common
│   │   ├── directed_graph.cc		# implementation of the directed graph
│   │   ├── generic_search.cc		# implementation of the generic search on graph (used for mrng)
│   │   └── handle_output.cc		# helper function for processing the output
│   ├── gnn						# implementation of the graph nearest neighbor search (construction, query)
│   │   └── gnn.cc
│   ├── mrng					# implementation of the monotonic relative neighborhood graph (construction)
│   │   └── mrng.cc
│   └── main.cc
├── include						# directory containing the header files
│   ├── directed_graph.hpp
│   ├── generic_search.hpp
│   ├── gnn.hpp
│   ├── handling.hpp
│   ├── mrng.hpp
│   └── set_utils.hpp				# overloading of the set operators for using set of pairs
├── testing						# directory containing the testing files (hyperparameter tuning)
│   ├── optimization.ipynb			# jupyter notebook for hyperparameter tuning including plots
│   ├── params.py					# functions for handling ctypes (lsh, cube, gnn, mrng)
│   └── python_connector.cc			# exposing the C++ functions to python
├── lib							# directory containing the shared library
│   ├── Makefile				# makefile for building only the shared library
│   └── graphsearch.so
├── MNIST					# directory for input and query data files
│   ├── input.dat
│   └── query.dat
├── graph_files				# directory for the saved graphs
│   ├── gnn_graph.bin
│   └── mrng_graph.bin
├── output					# directory for the result file
│   └── output.txt
├── common.mk				# main makefile
├── Makefile				# makefile for building the main program (including the shared library)
└── README.md				# this documentation file
├── ../exercise1/			# directory containing the source code for the first project
│   └── ...
```

# 2. Compilation

## 2.1. Main program `graphsearch`

Remain in the root directory of the <code>exercise2/</code> and then run the following command:

```bash
make
```

This will build the main program <code>graphsearch</code> and the shared library <code>graphsearch.so</code> in the <code>lib/</code> directory.

## 2.2. Shared library `graphsearch.so`

Remain in the root directory of the <code>exercise2/</code> and then run the following command:

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

After running the commands in [2.1.](#21-main-program-graphsearch), run the following at the root directory of the <code>exercise2/</code>:

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

To bridge c++ and python, we used `ctypes` [[2]](#references) and we created the `params.py` file which contains the functions for handling the ctypes for each algorithm. The `python_connector.cc` file exposes the C++ functions to python by returning primitive C types (c_int, c_char_p, c_POINTER, etc)
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

## 4.2. Graph Nearest Neighbor Search

### 4.2.1. Construction

LSH is used to construct the graph. For each point $p$ in the dataset, we find its $k$ nearest neighbors and we add an edge between $p$ and each of its neighbors. For LSH we use the following parameters derived from the hyperparameter tuning using the constraints: neighbors returned $\geq 50$, time $\leq 0.01$ second, $\min$ {maf}, $\min$ {time}:

| LSH |
|:------:|
| $k = 7$ |
| $L = 4$ |
| $table\_size = 15000$ |
| $w = 1815$ |
| $query = false$ |

Even though most of the times, the lsh algorithm returns at least $k=50$ neighbors, as the algorithm is probabilistic, we have no guarantee that it will always return such a number of neighbors. (WORKAROUND)

Average construction time: 50-70 seconds.

### 4.2.2. GNNS Query Search Algorithm

In comparison with the pseudo-code given, no greedy moves $T$ are used, it is checked if a node reached is better than its new neighbors (local optima).
We also stop the search if we return to a node that we have already visited (cycle). There is no need to sort distances at the end, as we use
an ordered set.

## 4.3. Monotonic Relative Neighborhood Graph

## 4.4 General Implementation Details

For the implementation of the algorithms with sets in most cases, we used the following data structures:

+ `std::[unordered_]multiset<pair<double,int>>` for storing the distances and the ids of the points (multiset because we can have duplicates in distance key). If the set is ordered we need extra a comparator function and if it is unordered we need extra a hash and an equal function (all are defined in `set_utils.hpp`).
+ `std::unordered_set<int>` for storing the ids of the points as multiset does not check for duplicate pairs or for checking if a point has been visited before.


# References

[1] LeCun, Y., Cortes, C., & Burges, C.. THE MNIST DATABASE
of handwritten digits. https://yann.lecun.com/exdb/mnist/

[2] ctypes — A foreign function library for Python
https://docs.python.org/3/library/ctypes.html

[3] Optuna - A hyperparameter optimization framework
https://optuna.org/
