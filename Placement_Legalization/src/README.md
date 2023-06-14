# Placement Legalization
Implement an existing algorithm, called “Abacus”, to legalize a given global placement result with minimal displacement.

## How to Compile
In `Placement_Legalization/src/`, enter the following command:
```
$ make
```
An executable file `hw4` will be generated in `Placement_Legalization/bin/`.

If you want to remove it, please enter the following command:
```
$ make clean
```

## How to Run
Usage:
```
$ ./hw4 [-o output_file] <aux file>
```
The default output filepath is `../output/<aux filename>.result`. If you want to specify the output file path, please add `-o <prefer output filepath>` when executing the command.

E.g.,
```
$ ./hw4 ../testcase/adaptec1/adaptec1.aux
```
The output filepath of this example is `../output/adaptec1.result`.

## How to Test
In `Placement_Legalization/src/`, enter the following command:
```
$ make test $name
```
It will build an executable file, test on testcase `$name`, and run a verifier to verify the answer.

E.g., test on adaptec1 and verify the answer.
```
$ make test adaptec1
```
