#include <stdio.h>
#include <stdlib.h>

//gcc -Wall hmacsha1.c -lssl -lcrypto

enum{
    MAX_KEY_SIZE=1024,
    MAX_DATA_SIZE=1024,
    BLOCK_SIZE=64
};

/*
def hmac_md5(key, msg):
    if len(key) > blocksize:
        key = md5(key).digest()
    key = key + bytearray(blocksize - len(key))
    o_key_pad = key.translate(trans_5C)
    i_key_pad = key.translate(trans_36)
    return md5(o_key_pad + md5(i_key_pad + msg).digest())
*/

void hmacsha1(int8_t * key, int key_size, int8_t * msg, int msg_size){
    if(key_size>BLOCK_SIZE){
        
    }
}

int main(int argc, char **argv) {
    /*
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
    */

    return 0;
}