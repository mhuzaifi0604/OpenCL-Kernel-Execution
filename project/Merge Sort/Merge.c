/*

Muhammad Huzaifa
20I-0604
Parallel & distributed Computing
Assignment # 03
Task # 02
Merge Sort using CPU + Sequential

*/
// Initializing Necessory Libraries
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#include<string.h>
#define Array_Size 1000

void Random_Initializer(float Arrayp[Array_Size], char* arr){
    srand((unsigned int)time(NULL));
    for(int i=0;i<Array_Size;i++){
        if(strcmp(arr, "unsort") == 0)
            Arrayp[i] = (1.0 + ((float)rand() / RAND_MAX) * (100.0 - 1.0));
        else if (strcmp(arr, "sort") == 0)
            Arrayp[i] = 0;
    }
}

void Print_Array(float Array[Array_Size]){
    for(int i=0;i<Array_Size;i++){
        printf("%.2f  ",Array[i]);
        if ((i%30) == 0 && i!=0){
            printf("\n");
        }
    }
    printf("\n");
}

int main(int argc, char** argv){
    double time_spent = 0.0; // Variable to store time taken by program
    // Declaring  static array
    float UnMerge[Array_Size], Merged[Array_Size];
    Random_Initializer(UnMerge, "unsort"); // Initializing Merge Sort array
    Random_Initializer(Merged, "sort"); // Initializing Merge Sort array
    // printf("Unsorted Array: \n"); 
    // Print_Array(UnMerge); // Printing Unsorted Array

    // Declaring Necessory OpenCL Variables
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_int error = CL_SUCCESS;
    cl_program program;
    cl_kernel kernel;
    
    // Declaring dynamic char variables to store names of platforms and devices
    char* platform_name = (char*)malloc(sizeof(char)*256);
    char* device_name = (char*)malloc(sizeof(char)*256);

    // getting platform id
    error = clGetPlatformIDs(1,&platform_id,NULL);
    if (error != CL_SUCCESS){
        printf("Error in getting platform id\n");
        return EXIT_FAILURE; // Rturning if got failure in getting platform id
    }
    // Getting Platform name
    error = clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,256,platform_name,NULL);
    if (error != CL_SUCCESS){
        printf("Error in getting platform name\n");
        return EXIT_FAILURE; // Rturning if got failure in getting platform name
    }
    printf("Platform Name: %s\n",platform_name);

    // getting Device Id
    error = clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_CPU,1,&device_id,NULL);
    if (error != CL_SUCCESS){
        printf("Error in getting device id\n");
        return EXIT_FAILURE; // Rturning if got failure in getting device id
    }
    // getting Device name
    error = clGetDeviceInfo(device_id,CL_DEVICE_NAME,256,device_name,NULL);
    if (error != CL_SUCCESS){
        printf("Error in getting device name\n");
        return EXIT_FAILURE; // Rturning if got failure in getting device name
    }
    printf("Device Name: %s\n",device_name);

    // Creating Context
    context = clCreateContext(NULL,1,&device_id,NULL,NULL,&error);
    if (error != CL_SUCCESS){ // checking if context got created or not
        printf("Error in creating context\n");
        return EXIT_FAILURE; // Rturning if got failure in creating context
    }
    // creating a commad queue for context
    command_queue = clCreateCommandQueue(context,device_id,CL_QUEUE_PROFILING_ENABLE,&error);
    if (error != CL_SUCCESS){ // checking if command queue got created or not
        printf("Error in creating command queue\n");
        return EXIT_FAILURE; // Rturning if got failure in creating command queue
    }
    printf("Context and Command Queue Created Successfully\n");

    // Reading Kernel File
    FILE* kernel_file = fopen("Kernel_Merge.cl","r");
    if (kernel_file == NULL){ // checking if kernel file got opened or not
        printf("Error in opening kernel file\n");
        return EXIT_FAILURE; // Rturning if got failure in opening kernel file
    }
    fseek(kernel_file,0,SEEK_END);
    size_t kernel_file_size = ftell(kernel_file);
    fseek(kernel_file,0,SEEK_SET);

    char* kernel_code = (char*)malloc(sizeof(char)*kernel_file_size + 1);
    if (!kernel_code){ // checking if kernel code got created or not
        printf("Error in creating kernel code\n");
        return EXIT_FAILURE; // Rturning if got failure in creating kernel code
    }
    // reading code from kernel_merge.cl to kernel code and type casting into const char *
    fread(kernel_code,1,kernel_file_size,kernel_file);
    kernel_code[kernel_file_size] = '\0'; // adding null character at the end of kernel code
    fclose(kernel_file); // closing kernel code file
    printf("\n::::::::::::::::::::::::::::: Kernel Code :::::::::::::::::::::::::::::\n\n%s\n\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n", kernel_code);

    // Creating Program with kernel code
    program = clCreateProgramWithSource(context,1,(const char**)&kernel_code,&kernel_file_size,&error);
    if (error != CL_SUCCESS){ // checking if program got created or not
        printf("Error in creating program\n");
        return EXIT_FAILURE; // Rturning if got failure in creating program
    }
    // Building Program
    error = clBuildProgram(program,1,&device_id,NULL,NULL,NULL);
    if (error != CL_SUCCESS){ // checking if program got build or not
        printf("Error in building program\n");
        char* build_log = (char*)malloc(sizeof(char)*kernel_file_size);
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, kernel_file_size, build_log, NULL);

        // Print or log the build log
        printf("OpenCL program build error:%s\n", build_log);
        return EXIT_FAILURE; // Rturning if got failure in building program
    }
    printf("Program Created and Build Successfully\n");

    // Creating Kernel
    kernel = clCreateKernel(program,"Merge_Sort",&error);
    if (error != CL_SUCCESS){ // checking if kernel got created or not
        printf("Error in creating kernel\n");
        return EXIT_FAILURE; // Rturning if got failure in creating kernel
    }
    printf("Kernel Created Successfully\n");

    // Creating Global Memory Buffers
    cl_mem Merge_Buffer = clCreateBuffer(context,CL_MEM_READ_WRITE,sizeof(int)*Array_Size,NULL,&error);
    if (error != CL_SUCCESS){ // checking if buffer got created or not
        printf("Error in creating buffer\n");
        return EXIT_FAILURE; // Rturning if got failure in creating buffer
    }
    printf("Buffer Created Successfully\n");

    // Writing Data to Buffer
    error = clEnqueueWriteBuffer(command_queue,Merge_Buffer,CL_TRUE,0,sizeof(float)*Array_Size,UnMerge,0,NULL,NULL);
    if (error != CL_SUCCESS){ // checking if data got written to buffer or not
        printf("Error in writing data to buffer\n");
        return EXIT_FAILURE; // Rturning if got failure in writing data to buffer
    }
    printf("Data Written to Buffer Successfully\n");

    // Setting Kernel Arguments
    int size = Array_Size;
    error = clSetKernelArg(kernel,0,sizeof(cl_mem),(void*)&Merge_Buffer);
    error |= clSetKernelArg(kernel,1,sizeof(int),(void*)&size);

    for (int i = 2; i <= Array_Size; i*= 2){
        for (int j = i / 2; j > 0; j/=2){
            error |= clSetKernelArg(kernel,2,sizeof(int),(void*)&j);
            error |= clSetKernelArg(kernel,3,sizeof(int),(void*)&i);
            size_t size = Array_Size;
             // Starting Timer
            clock_t start = clock();
            error |= clEnqueueNDRangeKernel(command_queue,kernel,1,NULL,&size,NULL,0,NULL,NULL);
            // Stopping Timer
            clock_t end = clock();
            time_spent += ((double)(end - start) / CLOCKS_PER_SEC) * 100.0;
            if (error != CL_SUCCESS){ // checking if kernel arguments got set or not
                printf("Error in setting kernel arguments\n");
                return EXIT_FAILURE; // Rturning if got failure in setting kernel arguments
            }
            clFinish(command_queue);
        }
    }

    // Reading from global buffers back into local
    error = clEnqueueReadBuffer(command_queue, Merge_Buffer, CL_TRUE, 0, sizeof(int) * Array_Size, Merged, 0, NULL, NULL);
    
    if (error != CL_SUCCESS){ // checking if data got read from buffer or not
        printf("Error in reading data from buffer\n");
        return EXIT_FAILURE; // Rturning if got failure in reading data from buffer
    }
    printf("Data Read from Buffer Successfully\n");
    // Print_Array(Merged); // Printing Sorted Array
    printf("Array Sorted Successfully\n");
    printf("Time Taken: %f ms\n", time_spent); // Printing Time Taken

    return 0;
}
