#include "proxy.h"

#include <sys/random.h>

/*
1. C←S : nonce
2. C calcula r=HMACSHA1(nonce||T,key)
3. C→S  :  r,T,login
4. S valida r calculando HMACSHA1(nonce||T,key)
5. C←S : "SUCCESS" o "FAILURE"
*/


int16_t nonce_counter;
//reads 6 bytes from /dev/urandom and appends 2 bytes from the nonce_counter to avoid repetitions.
int64_t generate_nonce(){
	int64_t r;
	if(getrandom(&r, sizeof(r)-sizeof(nonce_counter), 0)<sizeof(r)-sizeof(nonce_counter)){
		err(1,"unable to read from /dev/urandom\n");
	}
	r=r<<sizeof(nonce_counter)*8;
	r=r|nonce_counter;	
	nonce_counter++;
	return r;
}

int main(){
	int sockfd, connfd;
	int port=8080;
    char *serverip = "127.0.0.1";

    sockfd=setup_server(serverip,port);
	while(1){
		connfd=accept_new_client(sockfd);
		
		int64_t nonce=generate_nonce();
		send_nonce(connfd,nonce);
		FILE*accounts=fopen("accounts.txt","r");//TODO: check if file opens ok
		int result=recv_request(connfd,nonce,accounts);
		fclose(accounts);
		send_response(connfd,result);

		struct sockaddr_in client;
		socklen_t addrlen=sizeof(client);
		getsockname(connfd,(struct sockaddr *)&client,&addrlen);
		char client_ip[16];
		inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));

		fflush(stdout);
		if(result){
			printf("SUCCESS, %s from %s\n","todo",client_ip);
		}
		else{
			printf("FAILURE, %s from %s\n","todo",client_ip);
		}
		fflush(stdout);
	}
	//server never closes, it's stuck in the while loop.
	close_server(sockfd);
	exit(0);
}