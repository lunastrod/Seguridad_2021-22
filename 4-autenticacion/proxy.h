//std
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

//sockets
#include <arpa/inet.h>

//sha1
#include <openssl/sha.h>

//memcpy strcpy...
#include <string.h>

//time() for timestamps
#include <time.h>

/*
MACRO PARA PONER TRAZAS
usage: TDEB("ip:%s port%d",ip,port);
asi son mas faciles de quitar para hacer la entrega
*/
#define TDEB(...) fprintf(stderr,"DEBUG: "__VA_ARGS__); fprintf(stderr,"\n")


enum{
    BLOCK_SIZE=64,//512 bits, B
    HASH_SIZE=20,//sha1 length
    LOGIN_SIZE=256,
    RESPONSE_SIZE=8,
    KEY_SIZE=20
};

struct nonce_msg{//server
    int64_t nonce;
};

struct auth_request_msg{//client
    uint8_t r[HASH_SIZE];//hmacsha1
    char login[LOGIN_SIZE];//null terminated
    int64_t t;//timestamp
};

struct auth_response_msg{//server
    char response[RESPONSE_SIZE];//"SUCCESS" "FAILURE" null terminated
};

struct account{
    char login[LOGIN_SIZE];
    uint8_t key[KEY_SIZE];
};

int setup_client(char* ip, int port);
void close_client(int sockfd);

int setup_server(char* ip, int port);
int accept_new_client(int sockfd);
void close_server(int sockfd);

void send_nonce(int connfd, int64_t nonce);//server
int64_t recv_nonce(int connfd);//client returns nonce

void send_request(int connfd, int64_t nonce, struct account acc);//client computes HMACSHA1(nonce||T,key) inside
int recv_request(int connfd, int64_t nonce, FILE * accounts);//server validates the key inside, returns true/false

void hexstr_to_bytes(char * str, uint8_t * bytes, int str_size);
struct account search_account_file(FILE * f, char * login);//searches login in file, returns login-hmac

void send_response(int connfd, int response);//server response: true/false
int recv_response(int connfd);//client returns response: true/false

void hmacsha1(uint8_t * hmac_out, uint8_t * key_input, int key_size, uint8_t * msg, int msg_size);