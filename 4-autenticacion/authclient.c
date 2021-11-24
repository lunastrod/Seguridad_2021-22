#include "proxy.h"
/*
1. C←S : nonce
2. C calcula r=HMACSHA1(nonce||T,key)
3. C→S  :  r,T,login
4. S valida r calculando HMACSHA1(nonce||T,key)
5. C←S : "SUCCESS" o "FAILURE"
*/

int main(){    
	int sockfd;
	int port=8080;
    char *serverip = "127.0.0.1";
    sockfd=setup_client(serverip,port);

    int64_t nonce= recv_nonce(sockfd);
    printf("nonce: %ld\n",nonce);

    struct account a;
    char * key="3f786850e387550fdab836ed7e6dc881de23001b";
    hexstr_to_bytes(key,a.key,KEY_SIZE*2);
    strcpy(a.login,"pepe2");
    send_request(sockfd,nonce,a);

    close_client(sockfd);
}