#include <netinet/in.h>	
#include <stdio.h>
#include <err.h>
#include <arpa/inet.h>
#include <unistd.h>
    
int main(){    
    struct sockaddr_in sin;
	int sockfd;
	int port=8080;
    char *serverip = "127.0.0.1"; // string

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		err(1, "socket failed");
	}
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(serverip);
	sin.sin_port = htons(port);
 	if(connect(sockfd, (struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1){
		err(1, "connect failed");
 	}
    
    printf("conexion ok");
    char buf[1024]="hola";
    //fgets(buf,1024,sockfd);
    
    while(1){
        printf("sending...%s",buf);
        fflush(stdout);
        send(sockfd, buf, 5,0);
        sleep(1);
    }
    
        //...  // usa sockfd como si fuese un pipe full-duplex

    sleep(10);

    close(sockfd);
}