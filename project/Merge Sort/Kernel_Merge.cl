__kernel void Merge_Sort(__global float* array, const unsigned int n, const unsigned int j, const unsigned int k) {
    unsigned int i = get_global_id(0);
    unsigned int m = i ^ j;
    if ((m) > i) {
        if ((i & m) == 0 && array[i] > array[m]) {
            float temp = array[i];
            array[i] = array[m];
            array[m] = temp;
        }
        if ((i & m) != 0 && array[i] < array[m]) {
            float temp = array[i];
            array[i] = array[m];
            array[m] = temp;
        }
    }
};