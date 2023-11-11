#include <stdio.h>
#include <CL/cl.h>

// Function to print OpenCL error messages
void checkError(cl_int err, const char *message) {
    if (err != CL_SUCCESS) {
        printf("Error: %s (%d)\n", message, err);
        exit(EXIT_FAILURE);
    }
}

int main() {
    cl_int err;
    cl_platform_id platform_ids[10];
    cl_uint num_platforms;

    // Get the number of available platforms
    err = clGetPlatformIDs(10, platform_ids, &num_platforms);
    checkError(err, "Failed to get OpenCL platforms");

    // Print information about each platform
    for (cl_uint i = 0; i < num_platforms; i++) {
        printf("Platform %u:\n", i);

        // Get information about the platform
        size_t platform_name_size;
        err = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, 0, NULL, &platform_name_size);
        checkError(err, "Failed to get platform name size");

        char *platform_name = (char *)malloc(platform_name_size);
        err = clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, platform_name_size, platform_name, NULL);
        checkError(err, "Failed to get platform name");

        printf("  Name: %s\n", platform_name);
        free(platform_name);

        // Get the number of devices on this platform
        cl_uint num_devices;
        err = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
        checkError(err, "Failed to get the number of devices");

        // Allocate memory for the device IDs
        cl_device_id *devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
        if (devices == NULL) {
            printf("Error: Memory allocation for devices failed\n");
            exit(EXIT_FAILURE);
        }

        // Get the device IDs
        err = clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
        checkError(err, "Failed to get device IDs");

        // Print information about each device
        for (cl_uint j = 0; j < num_devices; j++) {
            printf("  Device %u:\n", j);

            // Get information about the device
            size_t device_name_size;
            err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, 0, NULL, &device_name_size);
            checkError(err, "Failed to get device name size");

            char *device_name = (char *)malloc(device_name_size);
            err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, device_name_size, device_name, NULL);
            checkError(err, "Failed to get device name");

            printf("    Name: %s\n", device_name);
            free(device_name);

            // Add more information about the device as needed
            // Example: Get and print the device type
            cl_device_type device_type;
            err = clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(cl_device_type), &device_type, NULL);
            checkError(err, "Failed to get device type");

            printf("    Type: ");
            if (device_type & CL_DEVICE_TYPE_CPU)
                printf("CPU ");
            if (device_type & CL_DEVICE_TYPE_GPU)
                printf("GPU ");
            if (device_type & CL_DEVICE_TYPE_ACCELERATOR)
                printf("Accelerator ");
            if (device_type & CL_DEVICE_TYPE_DEFAULT)
                printf("Default ");
            printf("\n");
        }

        // Free the allocated memory for devices
        free(devices);
    }

    return 0;
}
