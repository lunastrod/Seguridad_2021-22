#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

/*
int8_t key[20];
int64_t nonce;
int64_t t = time_t time(time_t *tloc);
*/


struct login_request {
    char login[256];
    int64_t t;
};

struct response {
    enum operations action;
    unsigned int counter;
    long waiting_time;
};


void hmacsha1(uint8_t * key_input, int key_size, uint8_t * msg, int msg_size);