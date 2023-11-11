/*

Muhammad Huzaifa
20I-0604
Parallel & distributed Computing
Assignment # 03
Task # 01
Matrix Multiplication using CPU + Sequential

*/

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define Matrix_Size 512

// funtion to initialize random values in 2D matrices
void Random_initializer(int Matrix[Matrix_Size][Matrix_Size], char matrix){
    // Initializing the Matrix with randonm values
    for (int i = 0; i < Matrix_Size; i++){
        for (int j = 0; j < Matrix_Size; j++){
            if (matrix == 'C')
                Matrix[i][j] = 0;
            else if (matrix == 'A' || matrix == 'B')
                Matrix[i][j] = rand() % 100; // storing random values in matrices
        }
    }
}

// function to print Matrices
void Print_Matrix(int Matrix[Matrix_Size][Matrix_Size], char matrix){
    printf("\n\t\t:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MATRIX - %c ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n", matrix);
    for (int i = 0; i < Matrix_Size; i++){ // Traversing to teh end of rows
        printf("\t[");
        for (int j = 0; j < Matrix_Size; j++){ // traversing to teh end of each colomn
            printf("%d ", Matrix[i][j]); // printing the matrix element
            if ((j%20) == 0){
                printf("]\n");
            }
            
        }
        printf("]\n\n");
    }
    printf("\n\t\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
}

void Seq_Matrix_Mult(int Matrix_A[][Matrix_Size], int Matrix_B[][Matrix_Size], int Matrix_C[][Matrix_Size]){
    for (int i = 0; i < Matrix_Size; i++){
        for (int j = 0; j < Matrix_Size; j++){
            for (int k = 0; k < Matrix_Size; k++){
                Matrix_C[i][j] += Matrix_A[i][k] * Matrix_B[k][j];
            }
        }
    }
}

int main (int argc, char** argv){
    if (argc != 2){
        printf("Usage ./Object File <Kernel-Device (CPU, GPU, SEQ)>\n");
        return EXIT_FAILURE; // exiting in case of invalid number of arguments
    }
    char * demand_device = argv[1];
    printf("\nDevice Demanded: %s\n", demand_device);
    srand(time (NULL));
    // Creating dynamic Matrices of Sixe 512 x 512
    // Creating Matrix A & B for Matrix Multiplication
    int Matrix_A[Matrix_Size][Matrix_Size];
    Random_initializer(Matrix_A, 'A');
    
    int Matrix_B[Matrix_Size][Matrix_Size];
    Random_initializer(Matrix_B, 'B');
    
    // Creating a Matrix C to store the result of matrix multiplication of A & B

    int Matrix_C[Matrix_Size][Matrix_Size];
    Random_initializer(Matrix_C, 'C');
    
    // Prinitng Matrices A & B
    // Print_Matrix(Matrix_A, 'A');
    // Print_Matrix(Matrix_B, 'B');

    printf("\nMatrix Assigning and printing Complete\n"
        "\n starting Clock time\n\n"
        "Getting Platforms and Device ID and Names\n\n"
    );     

    // Creating a variable to store the start time of the program
    

    // Declarinf Opencl variables
    cl_platform_id platform_ids;    // to get platform id
    cl_device_id device_id;         // to get device id
    cl_context context;             // to create context
    cl_command_queue command_queue; // to create command queue
    cl_int err = CL_SUCCESS;        // to check errors
    cl_kernel kernel;               // to create kernel

    // getting platform id
    err = clGetPlatformIDs(2, &platform_ids, NULL);
    if (err != CL_SUCCESS){
        printf("Error: Failed to get platform id\n");
        return EXIT_FAILURE; // exiting in case of failure on getting platform id
    }

    // creating a variable to store platform name
    char * platform_name = (char*)malloc(sizeof(char)*256);

    // getting platform name
    err = clGetPlatformInfo(platform_ids, CL_PLATFORM_NAME, 256, platform_name, NULL);
    if (err != CL_SUCCESS){
        printf("Error: Failed to get platform name\n");
        return EXIT_FAILURE; // exiting in case of failure on getting platform name
    }
    printf("\n Platform Name: %s\n", platform_name);

    //getting device ID
    if (strcmp(demand_device, "GPU") == 0 || strcmp(demand_device, "gpu") == 0){
        err = clGetDeviceIDs(platform_ids, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
        if (err != CL_SUCCESS){
            printf("Error: Failed to get GPU device id\n");
            return EXIT_FAILURE; // exiting in case of failure on getting device id
        }
    }
    else if (strcmp(demand_device, "CPU") == 0 || strcmp(demand_device, "cpu") == 0){
        err = clGetDeviceIDs(platform_ids, CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
        if (err != CL_SUCCESS){
            printf("Error: Failed to get CPU device id\n");
            return EXIT_FAILURE; // exiting in case of failure on getting device id
        }
    }else if (strcmp(demand_device, "SEQ") == 0 || strcmp(demand_device, "seq") == 0){
        printf("Sequential Execution\n");
        // starting clock time
        clock_t start_time = clock();
        Seq_Matrix_Mult(Matrix_A, Matrix_B, Matrix_C);
        // creating a variable to store the end time of the program
        // calculating the total time taken by the program in miliseconds
        printf("Sequential Execution Complete\n");
        // Print_Matrix(Matrix_C, 'C');
        clock_t end_time = clock();
        double total_time = (double)(end_time - start_time)/CLOCKS_PER_SEC;
        // printing total time taken by the program
        printf("\n\nTotal Time Taken: %f\n\n", total_time);
        return 0;
    }
    
    // creating a variable to store device name
    char * device_name = (char*)malloc(sizeof(char)*256); 

    // getting device name
    err = clGetDeviceInfo(device_id, CL_DEVICE_NAME, 256, device_name, NULL);
    if (err != CL_SUCCESS){
        printf("Error: Failed to get device name\n");
        return EXIT_FAILURE; // exiting in case of failure on getting device name
    }
    printf("\n Device Name: %s\n", device_name);

    // creating context
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS){
        printf("Error: Failed to create context\n");
        return EXIT_FAILURE; // exiting in case of failure on creating context
    }

    // creating command queue
    command_queue = clCreateCommandQueue(context, device_id, 0, &err);
    if (err != CL_SUCCESS){
        printf("Error: Failed to create command queue\n");
        return EXIT_FAILURE; // exiting in case of failure on getting creating command queue
    }
    printf("\n context && command Queue Created\n");

    // Creating and Defining the kernel
    // reading the kernel file
    FILE * kernel_file = fopen("Matrix_Mul_Kernel.cl", "r");
    if (kernel_file == NULL){
        printf("Error: Failed to open kernel file\n");
        return EXIT_FAILURE; // exiting in case of failure on opening kernel file
    }
    fseek (kernel_file, 0, SEEK_END);
    long kernel_file_size = ftell(kernel_file);
    fseek(kernel_file, 0, SEEK_SET);

    char * kernel_code = (char*)malloc(sizeof(char)*kernel_file_size + 1);
    if (!kernel_code){
        printf("Error: Failed to allocate memory to kernel code\n");
        return EXIT_FAILURE; // exiting in case of failure on allocating memory to kernel code
    }
    fread(kernel_code, 1, kernel_file_size, kernel_file);
    kernel_code[kernel_file_size] = '\0';
    printf("\n::::::::::::::::::::::::::::: Kernel Code :::::::::::::::::::::::::::::\n\n%s\n\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n", kernel_code);
    fclose(kernel_file);
    printf("Reading from Kernel File successfull!\n");

    // Creating Program For the kernel
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernel_code, &kernel_file_size, &err);
    if (err != CL_SUCCESS){
        printf("Error: Failed to create program\n");
        return EXIT_FAILURE; // exiting in case of failure on creating program
    }
    printf("Program Created Successfully!\n");

    // Building the program for the kernel
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (err != CL_SUCCESS){
        char logs[4096];
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(logs), logs, NULL);
        printf("Error: Failed to build program \n%s\n", logs);
        return EXIT_FAILURE; // exiting in case of failure on building program
    }
    printf("Program Built Successfully!\n");
    // Creating the kernel
    kernel = clCreateKernel(program, "Matrix_Multiplication", &err);
    if (err != CL_SUCCESS){
        printf("Error: Failed to create kernel\n");
        return EXIT_FAILURE; // exiting in case of failure on creating kernel
    }
    printf("Kernel Created Successfully!\n");
    // creating the memory objects
    cl_mem Matrix_A_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, Matrix_Size*Matrix_Size*sizeof(int), NULL, &err);
    cl_mem Matrix_B_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, Matrix_Size*Matrix_Size*sizeof(int), NULL, &err);
    cl_mem Matrix_C_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, Matrix_Size*Matrix_Size*sizeof(int), NULL, &err);
    printf("memory Objects Allocated Successfully!\n");

    // copy data from local variables to memory objects
    err = clEnqueueWriteBuffer(command_queue, Matrix_A_mem_obj, CL_TRUE, 0, Matrix_Size*Matrix_Size*sizeof(int), Matrix_A, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(command_queue, Matrix_B_mem_obj, CL_TRUE, 0, Matrix_Size*Matrix_Size*sizeof(int), Matrix_B, 0, NULL, NULL);
    printf("Data Copied to memory objects Successfully!\n");

    int matrix_size = Matrix_Size;
    // Adding kernel arguments and executing the kernel
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&Matrix_A_mem_obj);
    err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&Matrix_B_mem_obj);
    err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&Matrix_C_mem_obj);
    err = clSetKernelArg(kernel, 3, sizeof(int), (void*)&matrix_size);
    printf("Kernel Arguments Set Successfully!\n");

    clock_t start_time = clock();
    size_t global_size[] = {Matrix_Size, Matrix_Size};
    clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_size, NULL, 0, NULL, NULL);
    printf("Kernel Executed Successfully!\n");
    // reading the memory object from the kernel back into local work units
    err = clEnqueueReadBuffer(command_queue, Matrix_C_mem_obj, CL_TRUE, 0, Matrix_Size*Matrix_Size*sizeof(int), Matrix_C, 0, NULL, NULL);
    clock_t end_time = clock();
    int Csize = sizeof(Matrix_C)/sizeof(Matrix_C[0][0]);
    printf("Csize: %d\n", Csize);
    if(err != CL_SUCCESS){
        printf("Error: Failed to read buffer\n");
        return EXIT_FAILURE; // exiting in case of failure on reading buffer
    }
    // Waiting to finish OpenCl execution
    clFinish(command_queue);
    
    // freeing all dynamically allocated memory
    free(platform_name);
    free(device_name);
    free(kernel_code);
    clReleaseMemObject(Matrix_A_mem_obj);
    clReleaseMemObject(Matrix_B_mem_obj);
    clReleaseMemObject(Matrix_C_mem_obj);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    // creating a variable to store the end time of the program
    // calculating the total time taken by the program
    double total_time = (double)(end_time - start_time)/CLOCKS_PER_SEC * 1000.0;

    // printing teh resultant matrix C
    // Print_Matrix(Matrix_C, 'C');
    printf("Completed Matrix Multiplication Execution Using %s\n", demand_device);
    printf("\n\nTotal Time Taken: %f\n\n", total_time);

    return 0;
}
