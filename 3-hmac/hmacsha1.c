#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "string.h"

//gcc -Wall hmacsha1.c -lssl -lcrypto

enum{
    BLOCK_SIZE=64,//bytes, 512 bits, B
    HASH_SIZE=20//bytes, sha1 length
};


#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

void hmacsha1(uint8_t * key_input, int key_size, uint8_t * msg, int msg_size);
typedef unsigned char byte;

int main() {
    const int DataLen = 4;
    int i;

    byte* testdata = (byte *)malloc(DataLen);
    for (i=0; i<DataLen; i++){
        testdata[i] = 'a';
    }
    hmacsha1(testdata,DataLen,testdata,DataLen);
    return 0;
}

void print_hex(uint8_t * data, int len){
    for (int i=0; i<len; i++)
	    printf("%02x.",(unsigned char)data[i]);
    putchar('\n');
    putchar('\n');
}

void hmacsha1(uint8_t * key_input, int key_size, uint8_t * msg, int msg_size){
    const uint8_t ipad = 0x36;
    const uint8_t opad = 0x5c;

    uint8_t key[BLOCK_SIZE];// key
    uint8_t ipad_key[BLOCK_SIZE];// key ^ ipad
    uint8_t opad_key[BLOCK_SIZE];// key ^ opad
    uint8_t result[HASH_SIZE];

    SHA_CTX shactx;

    //padding key with 0s
    for(int i=0; i<BLOCK_SIZE; i++){
        key[i]=0;
    }
    //printf("key:\t");
    //print_hex(key,BLOCK_SIZE);

    if(key_size>BLOCK_SIZE){
        //SHA1(key_input,key_size,)
        //printf("hashing key...\n");
        SHA1_Init(&shactx);
        SHA1_Update(&shactx, key_input, key_size);
        SHA1_Final(key, &shactx);
        //key=sha1(key_input)
    } else{
        //printf("copying key...\n");
        memcpy(key,key_input,key_size);
        //key=key_input //(copy)
    }

    //printf("key:\t");
    //print_hex(key,BLOCK_SIZE);

    for(int i=0; i<BLOCK_SIZE; i++){
        ipad_key[i]=key[i] ^ ipad;
        opad_key[i]=key[i] ^ opad;
    }

    //printf("ipad_key:\t");
    //print_hex(ipad_key,BLOCK_SIZE);

    //printf("opad_key:\t");
    //print_hex(opad_key,BLOCK_SIZE);
    
    //H(key ^ ipad, msg)
    uint8_t in_hash[HASH_SIZE];
    SHA1_Init(&shactx);
    SHA1_Update(&shactx, ipad_key, BLOCK_SIZE);
    SHA1_Update(&shactx, msg, msg_size);
    SHA1_Final(in_hash, &shactx);

    //printf("in_hash:\t");
    //print_hex(in_hash,HASH_SIZE);

    //H(key ^ opad, H(key ^ ipad, msg))
    SHA1_Init(&shactx);
    SHA1_Update(&shactx, opad_key, BLOCK_SIZE);
    SHA1_Update(&shactx, in_hash, HASH_SIZE);
    SHA1_Final(result, &shactx);

    printf("result:\t");
    print_hex(result,HASH_SIZE);
}