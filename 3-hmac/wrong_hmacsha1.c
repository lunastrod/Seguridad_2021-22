#include <stdio.h>
#include <stdlib.h>

//gcc -Wall hmacsha1.c -lssl -lcrypto

enum{
    MAX_KEY_SIZE=1024,
    MAX_COMMAND_SIZE=1024
};

int main(int argc, char **argv) {
    if(argc!=3){
        fprintf(stderr,"argc error\n");
        fprintf(stderr,"usage: hmacsha1 data_file key_file\n");
        return -1;
    }
    //argv[1] data file, input to openssl
    //argv[2] key file, extract key, input to openssl

    FILE *key_file;
    char key[MAX_KEY_SIZE];

    /* opening file for reading */
    key_file = fopen(argv[2] , "r");
    if(key_file == NULL) {
        fprintf(stderr,"Error opening key_file\n");
        return(-1);
    }

    printf("%s",fgets(key, MAX_KEY_SIZE, key_file));
    fclose(key_file);

    
    char command[MAX_COMMAND_SIZE];

    //-binary returns binary data, xxd creates a hex dump of it.
    //I don't like the openssl dgst -hex format, it adds "(stdin)= "
    snprintf(command,MAX_COMMAND_SIZE,"openssl dgst -sha1 -binary -hmac '%s' < %s | xxd -p",key,argv[1]);

    printf("%s\n",command);
    fflush(stdout);
    system(command);

   return 0;
}