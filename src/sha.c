#include "sha.h"

void printb8(uint8_t x){
    for (int i = 7; i >= 0; i--) {putchar((x >> i) & 1 ? '1' : '0');}
}

void printb64(uint64_t x){
    for (int i = 63; i >= 0; i--) {putchar((x >> i) & 1 ? '1' : '0');}
}

int buildInput(FILE* fp, uint64_t inputs[]){
    memset(inputs, 0, 9*sizeof(uint64_t));

    uint8_t tmp[72] = {0};
    size_t count = fread(tmp, sizeof(uint8_t), 72, fp);

    int isContine = (count==72);
    if(isContine){
        for(int i=0; i<=8; i++){
            for(int k=0; k<=7; k++){
                inputs[i] |= ((uint64_t)(tmp[8*i+k])<<8*(k));
            }
        }
    }else{
        tmp[count] |= (uint8_t)0x06;
        tmp[71] |= (uint8_t)0x80;
        for(int i=0; i<=8; i++){
            for(int k=0; k<=7; k++){
                inputs[i] |= ((uint64_t)(tmp[8*i+k])<<8*(k));
            }
        }
    }
    return isContine;
}

uint8_t* sha3(FILE* fp){
    uint64_t kac[25] = {0};

}
