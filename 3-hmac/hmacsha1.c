#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include "string.h"

//gcc -Wall hmacsha1.c -lssl -lcrypto

enum{
    MAX_KEY_SIZE=1024,
    MAX_DATA_SIZE=1024,
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
    //testdata[DataLen-1]=0;
    hmacsha1(testdata,DataLen,testdata,DataLen);
    
    /*
    const int DataLen = 4;
    SHA_CTX shactx;
    byte digest[SHA_DIGEST_LENGTH];
    int i;

    byte* testdata = (byte *)malloc(DataLen);
    for (i=0; i<DataLen; i++) testdata[i] = 'a';

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, testdata, DataLen);
    SHA1_Final(digest, &shactx);

    for (i=0; i<SHA_DIGEST_LENGTH; i++)
	printf("%02x", digest[i]);
    putchar('\n');
    */
    return 0;
}

/*
function hmac (key, message)
    if (length(key) > blocksize) then
        key = hash(key) // llaves con más tamaño que el bloque se acortan
    end if
    if (length(key) < blocksize) then
        key = key ∥ [0x00 * (blocksize - length(key))] // llaves más cortas que el tamaño del bloque se agrandan con ceros (donde ∥ es la concatenación)
    end if
   
    o_key_pad = [0x5c * blocksize] ⊕ key // Cuando el tamaño del bloque es el de la función de hash subyacente
    i_key_pad = [0x36 * blocksize] ⊕ key // Donde ⊕ es el o exclusivo (XOR)
   
    return hash(o_key_pad ∥ hash(i_key_pad ∥ message)) // Donde ∥ es la concatenación
end function
*/

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

/*
int main(int argc, char **argv) {
    
    if(argc!=3){
        fprintf(stderr,"argc error\n");
        fprintf(stderr,"usage: hmacsha1 data_file key_file\n");
        return -1;
    }
    //argv[1] data file, input to openssl
    //argv[2] key file, extract key

    FILE *key_file;
    char key[MAX_KEY_SIZE];

    key_file = fopen(argv[2] , "r");
    if(key_file == NULL) {
        fprintf(stderr,"Error opening key_file\n");
        return(-1);
    }

    printf("%s",fgets(key, MAX_KEY_SIZE, key_file));
    fclose(key_file);
    

    return 0;
}
*/