# Homework2: Two-way Min-cut Partitioning
Use FM algorithm to implement two-way min-cut partitioning.

## Notice
This code has been modified to OOP on 2020.11.04.

This is not the same as report.

## How to Compile
In this directory, enter the following command:
```
$ make
```
It will generate the executable file "FM_Partitioner" in "Homework2_Two-way_Min-cut_Partitioning/bin/".

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
**In this directory, enter the following command:**
```
Usage: ../bin/<exe> <net file> <cell file> <output file>
```

e.g.
```
$ ../bin/FM_Partitioner ../testcases/p2-1.nets ../testcases/p2-1.cells ../output/p2-1.out
```

**In "Homework2_Two-way_Min-cut_Partitioning/bin/", enter the following command:**
```
Usage: ../<exe> <net file> <cell file> <output file>
```

e.g.
```
$ ./FM_Partitioner ../testcases/p2-1.nets ../testcases/p2-1.cells ../output/p2-1.out
```

## How to Test
In this directory, enter the following command:
```
$ make test $(id)
```
It will test on p2-$(id) and run verifier to verify the answer.

e.g. test on p2-1 and verify the answer
```
$ make test 1
```
