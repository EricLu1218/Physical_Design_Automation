# Homework4: Placement Legalization
Implement an existing algorithm, called “Abacus”, to legalize a given global placement result with minimal displacement.

## Notice
This code has been modified to OOP on 2021.02.27.

This is not the same as report.

## How to Compile
In this directory, enter the following command:
```
$ make
```
It will generate the executable file "hw4" in "Homework4_Placement_Legalization/bin/".

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
**In this directory, enter the following command:**
```
Usage: ../bin/<exe> <aux file>
```

e.g.
```
$ ../bin/hw4 ../testcase/adaptec1/adaptec1.aux
```

**In "Homework4_Placement_Legalization/bin/", enter the following command:**
```
Usage: ./<exe> <aux file>
```

e.g.
```
$ ./hw4 ../testcase/adaptec1/adaptec1.aux
```

## How to Test
In this directory, enter the following command:
```
$ make test $(name)
```
It will test on $(name) and run verifier to verify the answer.

e.g. test on adaptec1 and verify the answer
```
$ make test adaptec1
```
