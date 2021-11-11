#include <netinet/in.h>	
#include <stdio.h>
#include <err.h>
#include <arpa/inet.h>
#include <unistd.h>
    
int main(){    
    int fd, sockfd;
	struct sockaddr_in sin;
	struct sockaddr sclient;
	int port=8080;
	socklen_t addrlen;

	//...

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		err(1, "socket failed");
	}
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(port);
	if(bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		err(1, "bind failed");
	}
	if(listen(sockfd, 100) < 0){ //como mucho 100 clientes en la cola
		err(1, "listen failed");
	}
	for(;;){
		addrlen = sizeof(sclient);
		fd = accept(sockfd, &sclient, &addrlen);
		if(sockfd < 0){
			err(1, "accept failed");
		}

        while(1){
            char buf[1024];
            int nr;  
            nr=read(fd, buf, 1024);      
            if(nr<0){
                err(1,"read ha fallado");
            }
            else{
                warnx("client disc");
                break;
            }
            printf("eee%s\n",buf);
            fflush(stdout);
            sleep(1);
        }

		//... // usa fd como si fuese un pipe full-duplex

		close(fd);
	}
	close(fd); 
}