# OpenCL-Kernel-Execution
This repository contains multiple codes executing using OpenCl on CPU &amp; GPU. Sequential Codes for each program executed on CPU and GPU has also been written to compare the execution time of all from the same problem category.

# Setting up OpenCL on Docker

```
- docker-compose up -d master --build
- docker exec -it dockercl-master-1 bash
```

- or just run ```script.bat``` in terminal and get started

After getting a nice bash shell to the container traverse into the folder you need to check for executon and follow the following commands.

## Task # 01 Matrix Multiplication Using CPU
Assuming Matrix Multiplication is the desired folder
```
cd Matrix Multiplication
gcc Matrix_Mul_CPU.c -o matmul -lOenCL
./matmul <mode of execution> i.e., ./matmul CPU
```
After running the executable via accurate command line arguments you will get something like this:
#### CPU based Kernel Execution
<img src='/Pics/Screenshot_1.png' alt='CPU exec Output'>
As you can see in the screenshot above, the name of the platform and the name of the CPU device used for the execution of kernel code responsible for the multiplication of 2 matrices.

#### Sequential Code Execution
Similarly, Let's try and run treh sequential code and check for its execution time.
<img src='/Pics/Screenshot_2.png' alt='CPU exec Output'>

As you would have already notice that the execution time taken by a sequential code is rather high then the execution time taken by the CPU kernel code.

#### Implementation

Foe the implementation of the CPU based kernel execution, thorough steps have been taken to ensure proper and smooth execution. Here are the highlights of the code:

- Getting Platform ID and Platform name
<img src='/Pics/Screenshot_3.png' alt='CPU exec Output'>

- Setting Device Type, (In case of Sequential) & Getting Device Name
<img src='/Pics/Screenshot_4.png' alt='CPU exec Output'>

- Creating Context and Command Queues
<img src='/Pics/Screenshot_5.png' alt='CPU exec Output'>

- Reading Kernel Code from Kernel File
   This has two ways, you can either use a const char * string to store the code that kernel is going to execute and pass this to create kernel and program. Or you can create a saperate kernel file and read it to pass kernel code to create kernel and program. In my opinion the later is a better option. 
<img src='/Pics/Screenshot_6.png' alt='CPU exec Output'>

- Create & Build Program
<img src='/Pics/Screenshot_7.png' alt='CPU exec Output'>

- Finally Load Memory arguments, Execute Kernel and Store the Returned global memory object for resultant into your work unit's local memory object.

<img src='/Pics/Screenshot_8.png' alt='CPU exec Output'>

###### Note
- you can uncomment the matrix printing codes on line # 82, 83, 272 to print the input and resultant matrices to observe the output better.

### Graphic Visualization:

<strong>How to Run:</strong>
```
cd Matrix Multiplication
pip3 install -r requirements.txt (U can search for system specific issues on your own)
python3 graph.py

```

To visualize the difference in execution times of both CPU and SEQ matrix multiplication I executed the Matric_Mul_CPU.c file with both CPU and SEQ modes using python's subprocess.
Then I extracted the execution times from teh output of both mode executions and stored in 2 different Arrays.
Then I used matplotlib to plot the graph using both mode's execution time and here is teh final result.
<img src='/project/Matrix_Multiplication/CPU_vs_SEQ.png' alt='CPU exec Output'>

## Task # 02 Merge Sort Using CPU
```
cd Merge Sort
gcc Merge.c -o merge -lOenCL
./merge
gcc SEQ_Merge_Sort.c -o sort
./sort
```
After running the executable you will get something like this:

#### CPU based Kernel Execution

<img src='/Pics/Screenshot_9.png' alt='CPU exec Output'>
As you can see in the screenshot above, the name of the platform and the name of the CPU device used for the execution of kernel code responsible for the multiplication of 2 matrices.

#### Sequential Code Execution
Similarly, Let's try and run treh sequential code and check for its execution time.
<img src='/Pics/Screenshot_10.png' alt='CPU exec Output'>

As you would have already notice that the execution time taken by a sequential code is rather high then the execution time taken by the CPU kernel code.

## Visualizing the Results
Following are teh visualized results of the execution times in CPU and Sequential code executions:
<img src='/project/Merge Sort/CPU_vs_SEQ.png' alt='CPU exec Output'>