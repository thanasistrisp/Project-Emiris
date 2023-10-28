# Software Development for Algorithmic Problems - Winter Semester 2023-24

# Project 1 - Vector Search and Clustering in C/C++

Eleftheria Vrachoriti - 1115202000026

Athanasios Trispiotis - 1115202000194

# 1. Project structure

```txt
exercise1/
├── A/                          # directory for source and header files for LSH and Hypercube
│   ├── common/                     # directory for source files that are used by both `lsh` and `cube`
│   │   ├── brute_force.cc              # Brute force Nearest Neighbour implementation for comparison
│   │   ├── handle_binary.cc            # helper functions for reading data from input files
│   │   ├── hash_function.cc            # helper functions for LSH hash functions h_i
│   │   └── lp_metric.cc                # helper functions for lp metrics (e.g. euclidean metric)
│   │
│   ├── LSH/                        # directory for source files for LSH implementation
│   │   ├── handle_output.cc            # helper functions for `lsh` output
│   │   ├── helper_LSH.hpp              # header file for `handle_output.cc`
│   │   ├── lsh.cc                      # LSH implementation
│   │   ├── main.cc                     # `lsh` main function
│   │   └── Makefile
│   │
│   └── RandomProjection/           # directory for source files for Hypercube implementation
│       ├── binary_string.cc            # helper functions for handling binary strings
│       ├── handle_output.cc            # helper functions for `cube` output
│       ├── helper_RP.hpp               # header file for `handle_output.cc`
│       ├── hypercube.cc                # Hypercube implementation
│       ├── helper_cube.cc              # helper functions for Hypercube implementation
│       ├── main.cc                     # `cube` main function
│       └── Makefile
│
├── B/                          # directory for source and header files for KMeans
│   ├── cluster.conf                # configuration file for `cluster`
│   ├── helper.cc                   # helper functions for `cluster` output
│   ├── helper_kmeans.hpp           # header file for `helper.cc`
│   ├── kmeans.cc                   # KMeans implementation
│   ├── kmeans.hpp                  # header file for `kmeans.cc`, KMeans class definition
│   ├── kmeanspp.cc                 # KMeans++ implementation
│   ├── main.cc                     # `cluster` main function
│   ├── vector_utils.cc             # helper functions for numerical operations on vectors
│   ├── vector_utils.hpp            # header file for `vector_utils.hpp`
│   └── Makefile
│
├── include/                    # directory for header files used in all three programs
│   ├── binary_string.hpp           # header file for `binary_string.cc`
│   ├── brute_force.hpp             # header file for `brute_force.cc`
│   ├── defines.hpp                 # definition of constants used throughout the whole program
│   ├── hash_function.hpp           # header file for `hash_function.cc`
│   ├── hash_table.hpp              # HashTable template class definition and implementation
│   ├── helper.hpp                  # header file for `handle_binary.cc`
│   ├── hypercube.hpp               # header file for `hypercube.cc`, Hypercube class implementation
│   ├── list.hpp                    # List template class definition and implementation
│   ├── lp_metric.hpp               # header file for `lp_metric.cc`
│   └── lsh.hpp                     # header file for `lsh`, LSH class definition
│
├── MNIST/                      # directory for input and query data files
│   ├── input.dat
│   └── query.dat
│
├── output/                     # directory for result files
│
├── common.mk                   # main Makefile
└── README.md                   # this documentation file
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

After running the commands in [2.1.](#21-lsh), run the following at the same directory:

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

Alternatively, you can skip [2.1.](#21-lsh) and compile and execute `lsh` with the default arguments using only the following commands:

    cd A/LSH/
    make run-lsh

To check memory leaks with `valgrind`, use the following command:

    make valgrind-lsh

## 3.2. `cube`

After running the commands in [2.2.](#22-cube), run the following at the same directory:

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

Alternatively, you can skip [2.2.](#22-cube) and compile and execute `cube` with the default arguments using only the following commands:

    cd A/RandomProjection/
    make run-cube

To check memory leaks with `valgrind`, use the following command:

    make valgrind-cube

## 3.3. `cluster`

After running the commands in [2.3.](#23-cluster), run the following at the same directory:

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

Alternatively, you can skip [2.3.](#23-cluster) and compile and execute `cluster` using only the following commands:

    cd B/
    make run-cluster

To check memory leaks with `valgrind`, use the following command:

    make valgrind-cluster

# 4. Documentation

## 4.1. `lsh`

### Implementation details:

The LSH implementation is based on class `LSH`, which contains objects of type `HashTable`.

None of the STL data structures offers fixed table size for hash tables and custom hash functions and thus, the `HashTable` ADT had to be implemented from scratch.

To support this implementation, additional ADTs had to be implemented as well, such as `HashBucket` and `List`.

The `HashTable` has a fixed number of bucket chains, which are objects of class `List` containing `HashBucket` objects. Finally, each `HashBucket` object is a list of values of type `V`.

Our implementation uses templates to ensure usability for different data types.
<br></br>

Each `HashTable` $j$ is has a set of `HashFunction` objects, representing $h_i$, $i = 0, \ldots, k$ and a set of random factors $r_i$, $i = 0, \ldots, k$, so that the value of the amplified hash function $g_j$ can be computed.
<br></br>

### Other details:

The random factors $r_i$ that are multiplied with $h_i(p)$, $i = 1, \ldots, k$ are non-negative, according to the instructions given in the complimentary courses.
<br></br>

The Querying Trick has been implemented.
<br></br>

In Range Search, the bound of $20 \cdot L$ is not being used so that all approximate nearest neighbours within range are included in the output.

## 4.2. `cube`

### General details:

The Hypercube implementation is based on class `Hypercube`.

Data points in $R^d$ are projected to $R^{d'}$ using $d'$ lsh functions $h_i$ (p_proj) and then by using $d'$ $f_i$, they are mapped to $\{0, 1\}^{d'}$ uniformely (Hamming Hypercube).

One $f_i$ function projects an integer x (generated by lsh family) to $\{0, 1\}$ but for a specific $i$, it "remembers" if it has already been projected before (using a map).

The `HashTable` type defined in `hypercube.hpp` is a hash table that maps a binary string to a list of integers (the indices of the data points that have been projected to this binary string). It takes a binary string, it hashes it to its decimal value and then uses this value as a key to the map. For this reason, class `BinaryString` was created, that overloads the equal operator and the hash function to be able to use the `std::unordered_map` data structure. The max buckets can be about $2^{d'}$ but as most of them will be empty, we dynamically create them to save memory.

For a fixed query point, similarly as above we find its corresponding bucket to map. We find nearest neighbors in increasing hamming distance vertices (probes) by calculating permutations of the query to match each hamming distance until we reach threshold.

## 4.3. `cluster`

### General details:

To be able to find in which cluster a point belongs to to, but also which points a cluster has, we need two structures for $\Omicron(1)$ lookup: an array, which position $i$ indicates the point of dataset (zero indexed), its value `array[i]` its cluster and an array of set of points.

After initialing centroid with the KMeans++ algorithm, we need to assign each point to a cluster. We do this by first assigning all points to first cluster and then without touching the centroids, for all algorithms, we assign each point using Lloyd's to its nearest centroid-cluster.
<br></br>

Updating centroid is done using the MacQueen algorithm, which updates them each time a point is assigned to a cluster.

For this reason, two `update_centroids()` functions have been implemented. The first one updates all cluster centroids at once and is called only at the end of the Lloyd's algorithm main loop. The second function is called only when a point moves from one cluster to another; only the two affected clusters are updated based on an incremental method that takes advantage of the new and old number of points inside the two clusters, reducing the number of calculations needed.

For the old cluster:
$centroid_{new} = \frac{centroid_{old} \cdot |cluster_{old}| - p_{new}}{|cluster_{new}|}$

For the new cluster:
$centroid_{new} = \frac{centroid_{old} \cdot |cluster_{old}| + p_{new}}{|cluster_{new}|}$

where:

$centroid_{old}$, $centroid_{new}$: the vectors of the cluster's centroid before and after the addition / removal of $p_{new}$

$|cluster_{old}|, $|cluster_{new}|$: the size of the cluster before and after the addition / removal of $p_{new}$

$p_{new}$: the data point that changes cluster
<br></br>

In Silhouette metric of a point $i$, $b(i)$ is the average distance of $i$ to objects in the cluster of the 2nd closest centroid and *not* the smallest mean distance of i to all points in any other cluster.

### Lloyd's algorithm:

We assign each point to its nearest centroid and then update centroids. We repeat this until no point changes cluster (no other convergence criteria is used, as the number of iterations is small in most cases).

### Reverse Search using LSH or Hypercube:

Each cluster centroid is used as a query in Range Search using each of the two algorithms. All Approximate Nearest Neighbours retrieved that lie within the given radius are assigned to the cluster with the corresponding centroid. If there are conflicts, i.e. a data point is found to be lying in two query spheres at the same time, the cluster with the closest centroid is chosen.

The initial value for radius is $min\{dist(c_i, c_i)\}$, where $c_i$ and $c_j$ are the centroids of clusters $i$ and $j$ respectively and $i \neq j$.

The radius is doubled at the end of each loop, until it reaches threshold.

The best threshold for the query radius is estimated to be approximately $4 \cdot max\{dist(c_i, c_i)\}$ through various experiments that we did.

Before doubling the radius, the centroids are updated using the methods that were described [above](#general-details-1).

The convergence criteria are the same as the ones used in [Lloyd's algorithm](#lloyds-algorithm).

## 4.4. Parameters

The default parameters we found to be the most appropriate for each program are defined in each program's Makefile, e.g.:
```
cube_ARGS = -d ../../MNIST/input.dat -q ../../MNIST/query.dat -k 14 -M 200 -probes 50 -o ../../output/output.txt -N 5 -R 10000
```

The window size used in all programs is declared in `defines.hpp` and is equal to $1000$. Smaller values are are not recommended for the MNIST dataset of $60000$ points, especially in the LSH ANN problem where the Querying Trick is used.

### 4.4.1. `lsh`

Better resutls are yielded with higher values of `L`, as each data point has a higher probability to lie in the same bucket as the query. However, adding more hash tables increases the memory footprint and the querying time as well, so anything ranging from $3$ to $7$ should be enough for satisfying results with distances less than $2 \cdot trueDist$.

The value of parameter `k` determines the randomness of the hashed value that will be used for inserting elements inside the hash tables, but increasing this parameter over the range of $[4 - 10]$ doesn't necessarily improve the quality of the results.

### 4.4.2. `cube`

Better results are yielded with higher values of `M`, `probes`, `R` and lower values of `k`. Less probes are needed with lower `k` values as the buckets are exponentially smaller with more points in each bucket. For `M` candidates, the more are checked we have higher probability of finding the nearest neighbor(s), same for `R`. `M` should always be at least same as `N`, for better results (not so good in practice though) at least `2N`.

### 4.4.3. `cluster`

For the given dataset, `k` should be $10$, as there are $10$ different handwritten digits. The parameters are all declared in a configuration file (`cluster.conf`) and the update method in Makefile as above.

# References

[1] LeCun, Y., Cortes, C., & Burges, C.. THE MNIST DATABASE
of handwritten digits. https://yann.lecun.com/exdb/mnist/

[2] Andoni, A., & Indyk, P. (2006). Near-optimal hashing algorithms for approximate nearest neighbor in high dimensions. *2006 47th Annual IEEE Symposium on Foundations of Computer Science (FOCS’06)*. https://doi.org/10.1109/focs.2006.49

[3] Andoni, A., & Indyk, P. (2005). E^2LSH 0.1 User Manual. http://web.mit.edu/andoni/www/LSH/manual.pdf

[4] Avarikioti, G., Emiris, I. Z., Psarros, I., & Samaras, G. (2016). Practical linear-space Approximate Near Neighbors in high dimension. *arXiv preprint arXiv:1612.07405*. https://arxiv.org/abs/1612.07405

[5] Avarikioti, G. (2017). Geometric Proximity Problems in High Dimensions. *Pergamos, Institutional Repository / Digital Library of the University of Athens (UoA)*. https://pergamos.lib.uoa.gr/uoa/dl/object/1708336