#include "proxy.h"


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