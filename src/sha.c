#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "sha.h"

uint64_t lend(uint8_t* ptr){
    uint64_t res = 0;
    res =(uint64_t)ptr[0]|((uint64_t)ptr[1]<<8)|((uint64_t)ptr[2]<<16)|((uint64_t)ptr[3]<<24)|((uint64_t)ptr[4]<<32)|((uint64_t)ptr[5]<<40)|((uint64_t)ptr[6]<<48)|((uint64_t)ptr[7]<<56);
    return res;
}

uint8_t* sha(FILE* fp){
    
}
