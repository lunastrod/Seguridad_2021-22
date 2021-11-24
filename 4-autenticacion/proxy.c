#include "proxy.h"

void print_hex(uint8_t * data, int len){
    for (int i=0; i<len; i++){
	    printf("%02x.",(unsigned char)data[i]);
    }
    putchar('\n');
    putchar('\n');
}

void hmacsha1(uint8_t * hmac_out, uint8_t * key_input, int key_size, uint8_t * msg, int msg_size){
    printf("key:\t");
    print_hex(key_input,key_size);
    printf("msg:\t");
    print_hex(msg,msg_size);

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
    memcpy(hmac_out,result,HASH_SIZE);
}

//connects client to server
//returns: int sockfd
int setup_client(char* ip, int port) {
    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        fprintf(stderr,"Socket creation failed\n" );
        exit(1);
    }
    else {
        printf("Socket successfully created\n" );
    }

    // assign ip and port
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    // connect client to server
    while((connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0) {
        fprintf(stderr,"Connection with the server failed, retrying\n");
        sleep(1);
    }
    printf("Client conected to server\n");
    return sockfd;
}

//closes client
void close_client(int sockfd){
    if(close(sockfd) == 1) {
        fprintf(stderr,"Close failed\n");
        exit(1);
    }
}

//setup server listen
//returns: int sockfd
int setup_server(char* ip, int port){
    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        fprintf(stderr,"Socket creation failed\n");
        exit(1);
    }
    else{
        printf("Socket successfully created\n");
    }
    // assign ip and port
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    // bind to socket
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        fprintf(stderr,"Socket bind failed\n");
        exit(1);
    }
    else{
        printf("Socket successfully binded\n");
    }
    
    //listen to clients
    if ((listen(sockfd, 100)) != 0) {
        fprintf(stderr,"Listen failed\n");
        exit(1);
    } 
    else {
        printf("Server listening\n");
    }
    return sockfd;
}

//returns: connfd
int accept_new_client(int sockfd){
    int connfd = accept(sockfd, (struct sockaddr*)NULL, NULL); //Acepta un nuevo cliente
    if (connfd < 0) {
        printf("Server accept failed\n");
        exit(1);
    } else {
        printf("Server accepts the client\n");
    }
    return connfd;
}

//closes server
void close_server(int sockfd){
    if(close(sockfd) == 1) {
        fprintf(stderr,"Close failed\n");
        exit(1);
    }
}

/*

*/

void send_nonce(int connfd, int64_t nonce){
    struct nonce_msg msg;
    msg.nonce=nonce;
    send(connfd, &msg, sizeof(msg), 0);
}

int64_t recv_nonce(int connfd){
    struct nonce_msg msg;
    recv(connfd, &msg, sizeof(msg),0);
    return msg.nonce;
}

//writes sizeof(int64_t) bytes in bytes
void two_int64s_to_bytearr(uint8_t * bytes, int64_t a, int64_t b){
    //printf("a: %ld    b: %ld\nbytes:\t",a,b);

    memcpy(bytes,&a,sizeof(a));
    memcpy(&bytes[sizeof(a)],&b,sizeof(b));
    //print_hex(bytes,16);
}

//client computes HMACSHA1(nonce||T,key) inside
void send_request(int connfd, int64_t nonce, uint8_t * key, char * login){
    struct auth_request_msg msg;
    strncpy(msg.login, login, LOGIN_SIZE);
    msg.t=time(NULL);
    uint8_t hmac_data[sizeof(int64_t)+sizeof(int64_t)];
    two_int64s_to_bytearr(hmac_data,nonce,msg.t);
    hmacsha1(msg.r, key, KEY_SIZE, hmac_data, sizeof(hmac_data));
    send(connfd, &msg, sizeof(msg), 0);
}

//server validates the key inside, returns true/false
int recv_request(int connfd, int64_t nonce, u_int8_t * key){
    struct auth_request_msg msg;
    recv(connfd, &msg, sizeof(msg),0);
    uint8_t hmac_data[sizeof(int64_t)+sizeof(int64_t)];
    two_int64s_to_bytearr(hmac_data,nonce,msg.t);
    uint8_t hmac[HASH_SIZE];
    hmacsha1(hmac, key, KEY_SIZE, hmac_data, sizeof(hmac_data));

    printf("cmp:\n");
    print_hex(msg.r,HASH_SIZE);
    print_hex(hmac,HASH_SIZE);
    if(memcmp(msg.r,hmac , HASH_SIZE)==0){
        return 1;
    }
    return 0;
}

void send_response(int connfd, int response){
    struct auth_response_msg msg;
    if(response){
        strncpy(msg.response,"SUCCESS",RESPONSE_SIZE);
    }
    else{
        strncpy(msg.response,"FAILURE",RESPONSE_SIZE);
    }
    send(connfd, &msg, sizeof(msg), 0);
}


int recv_response(int connfd){
    struct auth_response_msg msg;
    recv(connfd, &msg, sizeof(msg),0);
    if(strncmp("SUCCESS",msg.response,RESPONSE_SIZE)==0){
        return 1;
    }
    if(strncmp("FAILURE",msg.response,RESPONSE_SIZE)==0){
        return 0;
    }
    return -1;
}