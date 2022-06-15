# Homework3: Fixed-outline Slicing Floorplan Design
Implement Stockmeyer algorithm to solve the fixed-outline floorplan design with a set of hard blocks.

## Notice
This code has been modified to OOP on 2021.01.29.

This is not the same as report.

## How to Compile
In this directory, enter the following command:
```
$ make
```
It will generate the executable file "hw3" in "Homework3_Fixed-outline_Slicing_Floorplan_Design/bin/".

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
**In this directory, enter the following command:**
```
Usage: ../bin/<exe> <hardblock file> <net file> <pl file> <floorplan file> <deadspace ratio>
```

e.g.
```
$ ../bin/hw3 ../testcase/n100.hardblocks ../testcase/n100.nets ../testcase/n100.pl ../output/n100.floorplan 0.2
```

**In "Homework3_Fixed-outline_Slicing_Floorplan_Design/bin/", enter the following command:**
```
Usage: ./<exe> <hardblock file> <net file> <pl file> <floorplan file> <deadspace ratio>
```

e.g.
```
$ ./hw3 ../testcase/n100.hardblocks ../testcase/n100.nets ../testcase/n100.pl ../output/n100.floorplan 0.2
```

## How to Test
In this directory, enter the following command:
```
$ make test $(name)
```
It will test on $(name) and run verifier to verify the answer.

e.g. test on n100 and verify the answer
```
$ make test n100
```
