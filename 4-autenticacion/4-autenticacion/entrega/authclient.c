#include "proxy.h"


/*
1. C←S : nonce
2. C calcula r=HMACSHA1(nonce||T,key)
3. C→S  :  r,T,login
4. S valida r calculando HMACSHA1(nonce||T,key)
5. C←S : "SUCCESS" o "FAILURE"

./authclient login key ip port
./authclient pepe 3f786850e387550fdab836ed7e6dc881de23001b 193.147.79.11 8838
asserts:
    4 args
    login: 1-256 chars null terminated
    key: 40 chars and a null, i don't care if the internal variable isn't null terminated.
        the key should be a hex value, I could test it but it doesn't really matter.
    ip and port: trust the errors from arpa/inet.h
*/

int main(int argc, char **argv){
    if(argc!=5){
        err(1,"usage: authclient login key ip port\n");
    }
    if(strlen(argv[1])>LOGIN_SIZE-1){
        err(1,"login too long\n");
    }
    struct account a;
    strncpy(a.login,argv[1],LOGIN_SIZE);

    if(strlen(argv[2])!=KEY_SIZE*2){
        err(1,"bad key size\n");
    }
    hexstr_to_bytes(argv[2],a.key,KEY_SIZE*2);

    char *serverip = argv[3];
    int port=strtol(argv[4],NULL,10);

	int sockfd=setup_client(serverip,port);

    int64_t nonce= recv_nonce(sockfd);
    send_request(sockfd,nonce,a);
    int response=recv_response(sockfd);

    close_client(sockfd);

    if(response){
        printf("AUTHENTICATION: SUCCESS\n");
        exit(0);
    }
    printf("AUTHENTICATION: FAILURE\n");
    exit(1);
}