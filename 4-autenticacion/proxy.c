#include "proxy.h"

void print_hex(uint8_t * data, int len){
    for (int i=0; i<len; i++){
	    printf("%02x.",(unsigned char)data[i]);
    }
    putchar('\n');
    putchar('\n');
}

void hmacsha1(uint8_t * hmac_out, uint8_t * key_input, int key_size, uint8_t * msg, int msg_size){
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

    if(key_size>BLOCK_SIZE){
        SHA1_Init(&shactx);
        SHA1_Update(&shactx, key_input, key_size);
        SHA1_Final(key, &shactx);
    } else{
        memcpy(key,key_input,key_size);
    }

    for(int i=0; i<BLOCK_SIZE; i++){
        ipad_key[i]=key[i] ^ ipad;
        opad_key[i]=key[i] ^ opad;
    }
    
    //H(key ^ ipad, msg)
    uint8_t in_hash[HASH_SIZE];
    SHA1_Init(&shactx);
    SHA1_Update(&shactx, ipad_key, BLOCK_SIZE);
    SHA1_Update(&shactx, msg, msg_size);
    SHA1_Final(in_hash, &shactx);

    //H(key ^ opad, H(key ^ ipad, msg))
    SHA1_Init(&shactx);
    SHA1_Update(&shactx, opad_key, BLOCK_SIZE);
    SHA1_Update(&shactx, in_hash, HASH_SIZE);
    SHA1_Final(result, &shactx);

    memcpy(hmac_out,result,HASH_SIZE);
}

//connects client to server
//returns: int sockfd
int setup_client(char* ip, int port) {
    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        err(1,"Socket creation failed\n");
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
        warn("Connection with the server failed, retrying");
        sleep(1);
    }
    printf("Client conected to server\n");
    return sockfd;
}

//closes client
void close_client(int sockfd){
    if(close(sockfd) == 1) {
        err(1,"Close failed\n");
    }
}

//setup server listen
//returns: int sockfd
int setup_server(int port){
    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        err(1,"Socket creation failed\n");
    }
    else{
        printf("Socket successfully created\n");
    }
    // assign ip and port
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // bind to socket
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        err(1,"Socket bind failed\n");
    }
    else{
        printf("Socket successfully binded\n");
    }
    
    //listen to clients
    if ((listen(sockfd, 100)) != 0) {
        err(1,"Listen failed\n");
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
        warn("Server accept failed\n");
    } else {
        printf("Server accepts the client\n");
    }
    return connfd;
}

//closes server
void close_server(int sockfd){
    if(close(sockfd) == 1) {
        err(1,"Close failed\n");
    }
}

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
    memcpy(bytes,&a,sizeof(a));
    memcpy(&bytes[sizeof(a)],&b,sizeof(b));
}

//client computes HMACSHA1(nonce||T,key) inside
void send_request(int connfd, int64_t nonce, struct account acc){
    struct auth_request_msg msg;
    strncpy(msg.login, acc.login, LOGIN_SIZE);
    msg.t=time(NULL);
    uint8_t hmac_data[sizeof(int64_t)+sizeof(int64_t)];
    two_int64s_to_bytearr(hmac_data,nonce,msg.t);
    hmacsha1(msg.r, acc.key, KEY_SIZE, hmac_data, sizeof(hmac_data));
    send(connfd, &msg, sizeof(msg), 0);
}

//server validates the key inside, returns true/false
int recv_request(int connfd, int64_t nonce, FILE * accounts, char * login_name){
    struct auth_request_msg msg;
    recv(connfd, &msg, sizeof(msg),0);
    strncpy(login_name,msg.login,LOGIN_SIZE);

    uint8_t hmac_data[sizeof(int64_t)+sizeof(int64_t)];
    two_int64s_to_bytearr(hmac_data,nonce,msg.t);
    uint8_t hmac[HASH_SIZE];

    struct account user=search_account_file(accounts, msg.login);
    if(user.login[0]=='\0'){
        fprintf(stderr,"user not found in database\n");
        return 0;
    }
    hmacsha1(hmac, user.key, KEY_SIZE, hmac_data, sizeof(hmac_data));
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

void hexstr_to_bytes(char * str, uint8_t * bytes, int str_size){
    char hex_buff[3] = {'0', '0', 0};
    for(int i=0; i<str_size; i+=2){
        hex_buff[0]=str[i];
        hex_buff[1]=str[i+1];
        bytes[i/2] = strtol(hex_buff, NULL, 16);
    }
}

struct account search_account_file(FILE * f, char * login){
    struct account acc;
    bzero(acc.login,LOGIN_SIZE);
    bzero(acc.key,KEY_SIZE);
    char buff[LOGIN_SIZE+1+KEY_SIZE*2+1+1];//login:hex_key\n\0
    char * file_login;
    char * file_key;
    int found=0;
    while(!found && fgets(buff,sizeof(buff),f)){
        file_login=strtok(buff,":");
        if(strncmp(login, file_login, LOGIN_SIZE)==0){
            found=1;
        }
    }
    if(!found){
        return acc;//returns acc struct full of '\0's
    }
    file_key=strtok(NULL,"\n");

    hexstr_to_bytes(file_key,acc.key,KEY_SIZE*2);
    strncpy(acc.login,login,LOGIN_SIZE);


    
    return acc;
}