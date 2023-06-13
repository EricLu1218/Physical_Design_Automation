# Fixed-outline Slicing Floorplan Design
Implement Stockmeyer algorithm to solve the fixed-outline floorplan design with a set of hard blocks.

## How to Compile
In `Fixed-outline_Slicing_Floorplan_Design/src/`, enter the following command:
```
$ make
```
An executable file `hw3` will be generated in `Fixed-outline_Slicing_Floorplan_Design/bin/`.

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
Usage:
```
$ ./hw3 <hardblock file> <net file> <pl file> <floorplan file> <deadspace ratio>
```

E.g.,
```
$ ./hw3 ../testcase/n100.hardblocks ../testcase/n100.nets ../testcase/n100.pl ../output/n100.floorplan 0.2
```

## How to Test
In `Fixed-outline_Slicing_Floorplan_Design/src/`, enter the following command:
```
$ make test $name $ratio
```
It will build an executable file, test on testcase `$name` with deadspace ratio `$ratio`, and run a verifier to verify the answer.

E.g., test on n100 with deadspace ratio 0.2 and verify the answer.
```
$ make test n100 0.2
```
