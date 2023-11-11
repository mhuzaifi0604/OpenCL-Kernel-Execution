#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to merge two subarrays of arr[]
void merge(float arr[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary arrays
    float L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    // Merge the temporary arrays back into arr[left..right]
    i = 0;   // Initial index of first subarray
    j = 0;   // Initial index of second subarray
    k = left; // Initial index of merged subarray

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Main function to perform merge sort
void mergeSort(float arr[], int left, int right) {
    if (left < right) {
        // Same as (left + right) / 2, but avoids overflow for large left and right
        int middle = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        // Merge the sorted halves
        merge(arr, left, middle, right);
    }
}

int main() {
    // Initialize an array of 1000 floats with random values
    // Starting Timer
    clock_t start = clock();
    float arr[1000];
    for (int i = 0; i < 1000; i++) {
        arr[i] = ((float)rand() / RAND_MAX) * 100.0;  // Random float between 0 and 100
    }

    // Print unsorted array
    printf("Unsorted Array:\n");
    for (int i = 0; i < 1000; i++) {
        printf("%.2f  ", arr[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    // Perform merge sort
    mergeSort(arr, 0, 999);

    // Print sorted array
    printf("Sorted Array:\n");
    for (int i = 0; i < 1000; i++) {
        printf("%.2f  ", arr[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    // Ending Timer
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
    printf("\nTime Taken: %f ms\n", time_spent);

    return 0;
}
