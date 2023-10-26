# Software Development for Algorithmic Problems - Winter Semester 2023-24

# Project 1 - Vector Search and Clustering in C/C++

Eleftheria Vrachoriti - 1115202000026

Athanasios Trispiotis - 1115202000194

# 1. Project structure

```txt
exercise1/
├── A/
│   ├── common/
│   │   ├── brute_force.cc
│   │   ├── handle_binary.cc
│   │   ├── hash_function.cc
│   │   └── lp_metric.cc
│   │
│   ├── LSH/
│   │   ├── handle_output.cc
│   │   ├── helper_LSH.hpp
│   │   ├── lsh.cc
│   │   ├── main.cc
│   │   └── Makefile
│   │
│   └── RandomProjection/
│       ├── binary_string.cc
│       ├── handle_output.cc
│       ├── helper_cube.cc
│       ├── helper_RP.hpp
│       ├── hypercube.cc
│       ├── main.cc
│       └── Makefile
│
├── B/
│   ├── cluster.conf
│   ├── helper.cc
│   ├── helper_kmeans.hpp
│   ├── kmeans.cc
│   ├── kmeans.hpp
│   ├── kmeanspp.cc
│   ├── main.cc
│   ├── Makefile
│   ├── vector_utils.cc
│   └── vector_utils.hpp
│
├── include/
│   ├── binary_string.hpp
│   ├── brute_force.hpp
│   ├── defines.hpp
│   ├── hash_function.hpp
│   ├── hash_table.hpp
│   ├── helper.hpp
│   ├── hypercube.hpp
│   ├── list.hpp
│   ├── lp_metric.hpp
│   └── lsh.hpp
│
├── MNIST/
│   ├── input.dat
│   └── query.dat
│
├── output/
│
├── common.mk
└── README.md
```

# 2. Compilation

## 2.1. `lsh`

Go to directory <code>exercise1/</code> and then run the following commands:

    cd A/LSH/
    make

## 2.2. `cube`

Go to directory <code>exercise1/</code> and then run the following commands:

    cd A/RandomProjection/
    make


## 2.3. `cluster`

Go to directory <code>exercise1/</code> and then run the following commands:

    cd B/
    make

## 2.4. `clean`

To remove dependency, object and executable files run the following command:

    make clean

at any of the three following directories:

+ `A/LSH`
+ `A/RandomProjection`
+ `B/`

# 3. Execution

## 3.1. `lsh`

After running the commands in [2.1.](#21-lsh), run the following:

    ./lsh -d <input file> -q <query file> -k <int> -L <int> -o <output file> -N <number of nearest> -R <double>

where:

+ `input file`: binary input data in the form that's specified in [[1]](#references)
+ `query file`: binary query data in the form that's specified in [[1]](#references)
+ `k`: number of LSH functions $h_i$ that will be used for defining $g$ functions
+ `L`: number of hash tables inside the LSH
+ `output file`: file for output
+ `N`: number of Approximate Nearest Neighbours of each query using LSH
+ `R`: radius for Range Search using LSH

If any of the numeric arguments aren't specified, the following values will be used:

| Argument | Default value |
|:------:|:------:|
| `k` | 4 |
| `L` | 5 |
| `N` | 1 |
| `R` | 10000 |


e.g.

    ./lsh -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 4 -L 5 -o ../../output/output.txt -N 1 -R 10000

## 3.2. `cube`

After running the commands in [2.2.](#22-cube), run the following:

    ./cube -d <input file> -q <query file> -k <int> -M <int> -probes <int> -o <output file> -N <number of nearest> -R <double>

where:

+ `input file`: binary input data in the form that's specified in [[1]](#references)
+ `query file`: binary query data in the form that's specified in [[1]](#references)
+ `k`: number of dimensions for Random Projection ($d'$)
+ `M`: maximum number of candidate data points that will be checked
+ `probes`: maximum number of Hypercube vertices that will be checked
+ `output file`: file for output
+ `N`: number of Approximate Nearest Neighbours of each query using Hypercube
+ `R`: radius for Range Search using Hypercube

e.g.

    ./cube -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 14 -M 200 -probes 50 -o ../../output/output.txt -N 5 -R 10000

If any of the numeric arguments aren't specified, the following values will be used:

| Argument | Default value |
|:------:|:------:|
| `k` | 14 |
| `M` | 10 |
| `probes` | 2 |
| `N`   | 1 |
| `R` | 10000 |

## 3.3. `cluster`

After running the commands in [2.3.](#23-cluster), run the following:

    ./cluster -i <input file> -c <configuration file> -o <output file> -complete <optional> -m <method: Classic or LSH or Hypercube>

where:

+ `input file`: input data in the form that's specified in [[1]](#references)
+ `configuration file`: configuration parameters for clustering using the Lloyd's method or Reverse Search using LSH or Hypercube. Have a look at `B/cluster.conf` file for more details.
+ `output file`: file for output
+ `-complete`: if specified, the data points inside each cluster will be appended at the end of the `output file`
+ `method`: `Classic` for Lloyd's method, `LSH` for Reverse Search using LSH or `Hypercube` for Reverse Search using Hypercube

e.g.

    ./cluster -i ../MNIST/input.dat -c cluster.conf -o ../output/cluster.txt -complete -m Classic
    ./cluster -i ../MNIST/input.dat -c cluster.conf -o ../output/cluster.txt -complete -m LSH
    ./cluster -i ../MNIST/input.dat -c cluster.conf -o ../output/cluster.txt -complete -m Hypercube

# References

[1] LeCun, Y., Cortes, C., & Burges, Christopher, THE MNIST DATABASE
of handwritten digits, https://yann.lecun.com/exdb/mnist/

[2] Andoni, A., & Indyk, P. (2006). Near-optimal hashing algorithms for approximate nearest neighbor in high dimensions. *2006 47th Annual IEEE Symposium on Foundations of Computer Science (FOCS’06)*. https://doi.org/10.1109/focs.2006.49

[3] Andoni, A., & Indyk, P. (2005). E^2LSH 0.1 User Manual. http://web.mit.edu/andoni/www/LSH/manual.pdf

[4] Avarikioti, G., Emiris, I. Z., Psarros, I., & Samaras, G. (2016). Practical linear-space Approximate Near Neighbors in high dimension. *arXiv preprint arXiv:1612.07405*. https://arxiv.org/abs/1612.07405