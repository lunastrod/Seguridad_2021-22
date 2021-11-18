#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

//gcc -Wall hmacsha1.c -lssl -lcrypto

enum{
    MAX_KEY_SIZE=1024,
    MAX_DATA_SIZE=1024,
    BLOCK_SIZE=64//bytes, 512 bits
};


#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

typedef unsigned char byte;

int main() {
    const int DataLen = 30;
    SHA_CTX shactx;
    byte digest[SHA_DIGEST_LENGTH];
    int i;

    byte* testdata = (byte *)malloc(DataLen);
    for (i=0; i<DataLen; i++) testdata[i] = 1;

    SHA1_Init(&shactx);
    SHA1_Update(&shactx, testdata, DataLen);
    SHA1_Final(digest, &shactx);

    for (i=0; i<SHA_DIGEST_LENGTH; i++)
	printf("%02x", digest[i]);
    putchar('\n');

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
/*
void hmacsha1(uint8_t * key_input, int key_size, uint8_t * msg, int msg_size){
    const uint8_t ipad = 0x36;
    const uint8_t opad = 0x5c;

    uint8_t key[BLOCK_SIZE];// key
    uint8_t ipad_key[BLOCK_SIZE];// key ^ ipad
    uint8_t opad_key[BLOCK_SIZE];// key ^ opad

    

    //padding key with 0s
    for(int i=0; i<BLOCK_SIZE; i++){
        key[i]=0;
    }
    if(key_size>BLOCK_SIZE){
        //SHA1(key_input,key_size,)
        //key=sha1(key_input) //hash
    } else{
        //key=key_input //(copy)
    }
    for(int i=0; i<BLOCK_SIZE; i++){
        ipad_key[i]=key[i] ^ ipad;
        opad_key[i]=key[i] ^ opad;
    }
    //H(key ^ opad, H(key ^ ipad, msg))
}
*/
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