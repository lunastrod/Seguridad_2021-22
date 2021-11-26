#include "proxy.h"
#include <sys/random.h>

/*
1. C←S : nonce
2. C calcula r=HMACSHA1(nonce||T,key)
3. C→S  :  r,T,login
4. S valida r calculando HMACSHA1(nonce||T,key)
5. C←S : "SUCCESS" o "FAILURE"
*/

/*
assert:
	1 or 2 args
	1st must be a filename, checked when opening for read
	2nd must be a number, optional arg
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

int main(int argc, char **argv){
	if(argc!=2 && argc!=3){
		err(1,"usage: authserver account-file [port]");
	}
	char * accounts_filename=argv[1];

	int port=9999;
	if(argc==3){
		port=strtol(argv[2],NULL,10);
	}

	int sockfd, connfd;
	
    sockfd=setup_server(port);
	while(1){
		connfd=accept_new_client(sockfd);
		//C←S : nonce
		int64_t nonce=generate_nonce();
		send_nonce(connfd,nonce);

		//C calcula r=HMACSHA1(nonce||T,key)
		//C→S  :  r,T,login
		//S valida r calculando HMACSHA1(nonce||T,key)
		FILE*accounts=fopen(accounts_filename,"r");
		if(accounts==NULL){
			err(1,"error opening %s", accounts_filename);
		}
		char login_name[LOGIN_SIZE];
		int result=recv_request(connfd,nonce,accounts,login_name);
		fclose(accounts);

		//C←S : "SUCCESS" o "FAILURE"
		send_response(connfd,result);

		//read the ip from the connfd (I know accept returns it, but I think it's easier this way)
		struct sockaddr_in client;
		socklen_t addrlen=sizeof(client);
		getsockname(connfd,(struct sockaddr *)&client,&addrlen);
		char client_ip[16];
		inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));

		//print result
		fflush(stdout);
		if(result){
			printf("SUCCESS, %s from %s\n",login_name,client_ip);
		}
		else{
			printf("FAILURE, %s from %s\n",login_name,client_ip);
		}
		fflush(stdout);
	}
	//server never closes, it's stuck in the while loop. I could implement a signal handler but it adds complexity
	close_server(sockfd);
	exit(0);
}