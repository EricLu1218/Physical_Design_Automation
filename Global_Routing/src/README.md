# Global Routing
Implement an existing algorithm to solve the global routing problem with a set of 2-pin nets while minimizing the total wirelength.

## How to Compile
In `Global_Routing/src/`, enter the following command:
```
$ make
```
An executable file `hw5` will be generated in `Global_Routing/bin/`.

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
Usage:
```
$ ./hw5 <input file> <output file>
```

E.g.,
```
$ ./hw5 ../testcase/ibm01.modified.txt ../output/ibm01.result
```

## How to Test
In `Global_Routing/src/`, enter the following command:
```
$ make test $name
```
It will build an executable file, test on testcase `$name`, and run a verifier to verify the answer.

E.g., test on ibm01 and verify the answer.
```
$ make test ibm01
```
