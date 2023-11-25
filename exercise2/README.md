# Software Development for Algorithmic Problems - Winter Semester 2023-24

# Project 2 - Graph Nearest Neighbour Search in C/C++

Eleftheria Vrachoriti - 1115202000026

Athanasios Trispiotis - 1115202000194

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
    └── graphsearch.so
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
│   ├── ...
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
./graphsearch -d <input file> -q <query file> -k <int> -E <int> -R <int> -N <int> -l <int, only for Search-on-Graph> -m <1 for GNNS, 2 for MRNG> -o <output file>
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

To perform hyperparameter analysis, you need to have `python` installed. Then, you need to install the following packages:

```bash
pip install optuna
pip install pandas
pip install matplotlib
pip install botorch
```

Then, you can open the `optimization.ipynb` jupyter notebook in <code>testing</code> directory and run the cells. The notebook contains the hyperparameter tuning for the following algorithms:

+ GNNS
+ MRNG
+ LSH
+ Cube

The results are already ran and saved in the above notebook.

To bridge c++ and python, we used `ctypes` [[2]](#references) and we created the `params.py` file which contains the functions for handling the ctypes for each algorithm. The `python_connector.cc` file exposes the C++ functions to python by returning primitive C types (c_int, c_char_p, c_POINTER, etc)
using `extern "C"`, a linkage specification.

### 3.2.2. Analysis


# 4. Documentation



# References

[1] LeCun, Y., Cortes, C., & Burges, C.. THE MNIST DATABASE
of handwritten digits. https://yann.lecun.com/exdb/mnist/

[2] ctypes — A foreign function library for Python
https://docs.python.org/3/library/ctypes.html

[3] Optuna - A hyperparameter optimization framework
https://optuna.org/
