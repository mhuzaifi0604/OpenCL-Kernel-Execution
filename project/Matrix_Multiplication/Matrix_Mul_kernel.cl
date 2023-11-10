__kernel void Matrix_Multiplication(__global int* A, __global int* B, __global int * C, int size){
    int i = get_global_id(0);
    int j = get_global_id(1);
    int sum = 0;
    for (int k = 0; k < size; k++){
        sum += A[i*size + k] * B[k*size + j];
    }
    C[i*size + j] = sum;
}