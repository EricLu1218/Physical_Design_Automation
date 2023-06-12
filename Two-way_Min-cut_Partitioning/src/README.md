# Two-way Min-cut Partitioning
Implement Fiduccia–Mattheyses algorithm to solve the problem of two-way min-cut partitioning.

## How to Compile
In `Two-way_Min-cut_Partitioning/src/`, enter the following command:
```
$ make
```
An executable file `FM_Partitioner` will be generated in `Two-way_Min-cut_Partitioning/bin/`.

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
Usage: 
```
$ ./FM_Partitioner <net file> <cell file> <output file>
```

E.g.,
```
$ ./FM_Partitioner ../testcases/p2-1.nets ../testcases/p2-1.cells ../output/p2-1.out
```

## How to Test
In `Two-way_Min-cut_Partitioning/src/`, enter the following command:
```
$ make test ${name}
```
It will build an executable file, test on testcase ${name}, and run a verifier to verify the answer.

E.g., test on p2-1 and verify the answer
```
$ make test p2-1
```
