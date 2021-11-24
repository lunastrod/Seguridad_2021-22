#include "proxy.h"
/*
1. C←S : nonce
2. C calcula r=HMACSHA1(nonce||T,key)
3. C→S  :  r,T,login
4. S valida r calculando HMACSHA1(nonce||T,key)
5. C←S : "SUCCESS" o "FAILURE"
*/

int main(){    
	int sockfd, connfd;
	int port=8080;
    char *serverip = "127.0.0.1";
    sockfd=setup_server(serverip,port);
	connfd=accept_new_client(sockfd);

	int64_t nonce=0;
	send_nonce(connfd,nonce);
	uint8_t key[KEY_SIZE];
	bzero(key,KEY_SIZE);
	int result=recv_request(connfd,nonce,key);
	printf("result: %d\n",result);
	


	close_server(sockfd);
}