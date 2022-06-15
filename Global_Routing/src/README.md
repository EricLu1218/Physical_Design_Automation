# Homework5: Global Routing
Implement an existing algorithm to solve the global routing problem with a set of 2-pin nets while minimizing the total wirelength.

## Notice
This code has been modified to OOP on 2021.03.03.

This is not the same as report.

## How to Compile
In this directory, enter the following command:
```
$ make
```
It will generate the executable file "hw5" in "Homework5_Global_Routing/bin/".

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
**In this directory, enter the following command:**
```
Usage: ../bin/<exe> <modified.txt file> <result file>
```

e.g.
```
$ ../bin/hw5 ../testcase/ibm01.modified.txt ../output/ibm01.result
```

**In "Homework5_Global_Routing/bin/", enter the following command:**
```
Usage: ./<exe> <modified.txt file> <result file>
```

e.g.
```
$ ./hw5 ../testcase/ibm01.modified.txt ../output/ibm01.result
```

## How to Test
In this directory, enter the following command:
```
$ make test $(name)
```
It will test on $(name) and run verifier to verify the answer.

e.g. test on ibm01 and verify the answer
```
$ make test ibm01
```
