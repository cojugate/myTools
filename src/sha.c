#include "sha.h"

static const int MAGIC[25]={0,1,62,28,27,36,44,6,55,20,3,10,43,25,39,41,45,15,21,8,18,2,61,56,14};
static const uint64_t RC[24] = {
    0x0000000000000001ULL,
    0x0000000000008082ULL,
    0x800000000000808AULL,
    0x8000000080008000ULL,
    0x000000000000808BULL,
    0x0000000080000001ULL,
    0x8000000080008081ULL,
    0x8000000000008009ULL,
    0x000000000000008AULL,
    0x0000000000000088ULL,
    0x0000000080008009ULL,
    0x000000008000000AULL,
    0x000000008000808BULL,
    0x800000000000008BULL,
    0x8000000000008089ULL,
    0x8000000000008003ULL,
    0x8000000000008002ULL,
    0x8000000000000080ULL,
    0x000000000000800AULL,
    0x800000008000000AULL,
    0x8000000080008081ULL,
    0x8000000000008080ULL,
    0x0000000080000001ULL,
    0x8000000080008008ULL
};

static uint64_t rot(uint64_t num, int n){if(n==0){return num;}else{return (num<<n)|(num>>(64-n));}}

static int buildInput(FILE* fp, uint64_t inputs[]){

    uint8_t tmp[72] = {0};
    size_t count = fread(tmp, sizeof(uint8_t), 72, fp);

    int isFull = (count==72);
    if(isFull){
        for(int i=0; i<=8; i++){
            for(int k=0; k<=7; k++){
                inputs[i] ^= ((uint64_t)(tmp[8*i+k])<<8*(k));
            }
        }
    }else{
        tmp[count] |= (uint8_t)0x06;
        tmp[71] |= (uint8_t)0x80;
        for(int i=0; i<=8; i++){
            for(int k=0; k<=7; k++){
                inputs[i] ^= ((uint64_t)(tmp[8*i+k])<<8*(k));
            }
        }
    }
    return count;
}

static uint64_t* theta(uint64_t inputs[]){
    uint64_t c[5] = {0};
    for(int i=0; i<=4; i++){c[i]=inputs[i]^inputs[i+5]^inputs[i+10]^inputs[i+15]^inputs[i+20];}
    uint64_t d[5] = {0};
    for(int i=0; i<=4; i++){d[i]=c[(i+4)%5]^rot(c[(i+1)%5], 1);}
    for(int y=0; y<=4; y++){for(int x=0; x<=4; x++){inputs[x+5*y]=inputs[x+5*y]^d[x];}}
    return inputs;
}

static uint64_t* rho(uint64_t inputs[]){for(int i=0; i<=24; i++){inputs[i]=rot(inputs[i], MAGIC[i]);}return inputs;}

static uint64_t* pi(uint64_t inputs[]){
    uint64_t b[25]={0};
    for(int y=0; y<=4; y++){for(int x=0; x<=4; x++){b[y+5*((2*x+3*y)%5)]=inputs[x+5*y];;}}
    memcpy(inputs, b, sizeof(b));
    return inputs;
}

static uint64_t* chi(uint64_t inputs[]){
    uint64_t b[25]={0};
    for(int y=0; y<=4; y++){for(int x=0; x<=4; x++){b[x+5*y]=inputs[x+5*y]^(~(inputs[(x+1)%5+5*y])&inputs[(x+2)%5+5*y]);}}
    memcpy(inputs, b, sizeof(b));
    return inputs;
}

static uint64_t* iota(uint64_t inputs[], int round){
    inputs[0]^=RC[round];
    return inputs;
}

static uint64_t* keccak(uint64_t inputs[]){for(int round=0; round<=23; round++){inputs = iota(chi(pi(rho(theta(inputs)))), round);}return inputs;}

void sha3_512(FILE* fp, uint8_t digest[]){
    uint64_t state[25] = {0};

    while(buildInput(fp, state) == 72){
        keccak(state);
    }keccak(state);

    for (int i=0; i<8; i++) {for(int k=0; k<8; k++){digest[i*8+k]=(state[i]>>(8*k))&0xff;}}
}
