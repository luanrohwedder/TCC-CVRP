# Implementation of the Genetic Algorithm and Memetic Algorithm for the CVRP

This is a basic implementation of the Genetic Algorithm and Memetic Algorithm to solve
the Capacitated Vehicle Routing Problem (CVRP), developed for an undergraduate thesis.

The goal of this algorithm is to compare the performance of two approaches: the Genetic Algorithm (GA)
and the Memetic Algorithm (MA), which incorporates a local search to improve solutions. In addition, the algorithm includes some enhancements with openMP that can be enabled to achieve better execution time.

## Requirements

To build and run this project, you need the following:

- `C++17`
- `Makefile`
- `OpenMP` (optional)

## Compiling

To compile the project, `Makefile` is required.

In the project root, run:

```console
make USE_OPENMP=0
```

The `USE_OPENMP` flag controls wheter OpenMP is used during compilation:

- `0` - compile without OpenMP.
- `1` - compile with OpenMP.

## Running

To execute the algorithm, simply run:

```console
./bin/ex PATH_TO_DATASET OPTIONS
```

The following options are available:

```text
[-p] Sets the population size. Default is 25
[-g] sets the number of generations. Default is 1000
[-s] sets the limit of stagnation. Default is 200
[-a] Selects the algorithm: GA or MA. Default is GA
```
