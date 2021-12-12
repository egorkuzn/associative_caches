#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <x86intrin.h>
#include <limits>

#define L1 131072 // L1 data = 128 KB
#define L2 2097152 // L2 = 2 MB
#define L3 4194304 // L3 = 4 MB
#define BLOCKS_COUNT 32
#define K 10

size_t takeTime(size_t* array, size_t size){
// cash warming up
    for(size_t k = 0, i = 0; i < size * K; ++i){
        if(k == K)
            std::cout << "_";
        k = array[k];
    }
    size_t start, end, time = std::numeric_limits<size_t>::max();
    for(size_t repeate = 0; repeate < K; ++repeate){
        //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
        start = __rdtsc();
        for (size_t k = 0, i = 0; i < size; ++i){
            if(k == K)
                std::cout << "_";
            k = array[k];
        }
        end = __rdtsc();
        //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
        if (time > end - start)
            time = end - start;
    }
    return time / size;
}

void createCirculation(size_t* array, size_t offset, uint8_t i){
    for(uint8_t j = 0; j < i; ++j)
        for(size_t z = 0; z < offset / i; ++z)
            array[j * offset + z] = (j + 1) * offset + z;
    for(size_t z = 0; z < offset / size_t(i); ++z)
        array[(i - 1) * offset + z] = z + 1;
    array[(i - 1) * offset + offset / i - 1] = 0;
}

int main(){
    size_t size = (L1 + L2 + L3) / sizeof(size_t);
    size_t offset = size;
    std::ofstream dataset;
    dataset.open("dataset.txt", std::ios::out | std::ios::app);
    for(u_int8_t i = 1; i <= BLOCKS_COUNT; ++i){
        std::cout << (int)i;
        size_t* array = new size_t [offset * i];
// filling of our array
        createCirculation(array, offset, i);
// outputing of merge result in dataset.txt source file
        dataset << takeTime(array, size) << std::endl;
        std::cout << " DONE" << std::endl;
        delete[] array;
    }
    dataset.close();
    return 0;
}
