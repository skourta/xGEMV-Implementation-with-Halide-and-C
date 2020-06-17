# xGEMV General Matrix-Vector Multiplication Implementation with Halide and C

This is an implementation of a BLAS2 (Basic Linear Algebra Subprograms) routine called **xGEMV** or **General Matrix-Vector Multiplication**, its corresponding formula is the following: 
$$
y = \alpha A x + \beta y
$$
- [xGEMV General Matrix-Vector Multiplication Implementation with Halide and C](#xgemv-general-matrix-vector-multiplication-implementation-with-halide-and-c)
  - [1.1. Requirements](#11-requirements)
  - [1.2. Build](#12-build)
  - [1.3. Usage](#13-usage)
    - [1.3.1. Arguments](#131-arguments)
    - [1.3.2. Execution](#132-execution)
      - [1.3.2.1. C implementation](#1321-c-implementation)
      - [1.3.2.2. Halide implementation](#1322-halide-implementation)
    - [1.3.3. Comparison](#133-comparison)
      - [1.3.3.1. Using the script](#1331-using-the-script)
      - [1.3.3.2. Choosing your own values inside Code](#1332-choosing-your-own-values-inside-code)

## 1.1. Requirements
This project is based on Halide and C so you will need to install Halide by following [the official guide](https://github.com/halide/Halide/blob/master/README.md) you can either use the binaries or install from source.

## 1.2. Build

1. First thing you need to do is clone this repository
```
git clone 
```
2. Positing yourself in the root of the project:
```
cd   
```
3. Run the make command to build the files, the make command takes the Halide path as an argument called halidePath:
```
make halidePath="/home/skourta/development/Halide/"
```
## 1.3. Usage
### 1.3.1. Arguments
There are 2 programs in this project, the Halide implementation and the C one, both provide the same arguments:
1. **debug**: the first argument is a debug flag by setting this argument to a non 0 value, the results will be displayed in the terminal. Its default value is 0, in which case only the execution time will be displayed. `(default 0)`
2. **aheight**: the 2nd argument is the Height (Number of rows) of the Matrix, it is also the size of the Y vector. `(default 5)`
3. **awidth**: the 3rd argument is the Width (Number of columns) of of the Matrix, it is also the size of the X vector.`(default 10)`
4. **alpha**: the value of alpha parameter. `(default 2)`
5. **beta**: the value of beta parameter. `(default 3)`
5. **random**: 
   - if **0** the values of the roms will be the row number for the matrix, while the vectors will go from 0 to their size. `default`
   - If **1** all the values will be generated randomly between -50 and 50.
6. **saveFile**: 
   - if **0** The results will not be saved locally. `default`
   - If **1** the results will be saved in local files in the results directory:
     - `halide_results.txt` for the **Halide** results.
     - `c_results` : for the **C** results.

### 1.3.2. Execution
#### 1.3.2.1. C implementation
- Execution with default values:  
``` 
bin/c 
```
- Execution with arguments:

``` 
bin/c debug aheight awidth alpha beta random saveFile

#for example
bin/c 1 10 5 4 5 0 1
```
#### 1.3.2.2. Halide implementation
You need to have the **LD_LIBRARY_PATH** set to the Halide binaries if you don't please use the following:
- Set a variable to hold the Halide path:
```
halidePath="/home/skourta/development/Halide/"

```
- Execution with default values:  
``` 
LD_LIBRARY_PATH=$halidePath/bin bin/halide  0 500 1000 4 5 0 1
```
- Execution with arguments:
``` 
LD_LIBRARY_PATH=$halidePath/bin bin/halide debug aheight awidth alpha beta random saveFile

#for example
LD_LIBRARY_PATH=$halidePath/bin bin/halide  0 500 1000 4 5 0 1
```

### 1.3.3. Comparison
#### 1.3.3.1. Using the script
All you need to do is give the scipt `compare.sh` execution permissions:
```
chmod +x compare.sh
```
then run the script while providing the Halide path:
```
./compare.sh -p "/home/skourta/development/Halide/"
```
or using the variable we set earlier:
```
./compare.sh -p $halidePath
```

The script will display the execution times of both implementations and check if the results are the same
#### 1.3.3.2. Choosing your own values inside Code
You will need to run both implemetations with the values you want (refer to the guide above to see how), executing the programs will display their corresponding execution time. Then run the following command to test if the results are identical:
```
diff -s -q results/c_results.txt results/halide_result.txt
```