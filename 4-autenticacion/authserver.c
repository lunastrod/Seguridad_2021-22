#include <netinet/in.h>	
#include <stdio.h>
#include <err.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
1. C←S : nonce
2. C calcula r=HMACSHA1(nonce||T,key)
3. C→S  :  r,T,login
4. S valida r calculando HMACSHA1(nonce||T,key)
5. C←S : "SUCCESS" o "FAILURE"

Login es un nombre de usuario, de como mucho 255 caracteres, terminado en un carácter nulo. Siempre se envían 256 bytes para este campo.

Key es una clave de 20 bytes. El servidor tiene que conocer la clave de todos los clientes para autenticarlos.

T es un número entero sin signo 64 bits, little endian. Contiene una marca de tiempo con el "tiempo UNIX" de la máquina obtenida con la llamada al sistema time(2). Se considerará que un mensaje es antiguo si es de hace más de 5 minutos. Esta ventana de tiempo determina el nivel de sincronización de los relojes de cliente y servidor.

Nonce es un número entero sin signo 64 bits, little endian. Su valor debe ser lo más aleatorio posible. El servidor no podrá reutilizar los nonces usados en los últimos 10 minutos.

El resultado final de la autenticación se indica en el mensaje del paso 5, con una cadena de caracteres ASCII, "SUCCESS" si la autenticación es correcta, "FAILURE" si la autenticación es incorrecta. Estas strings tienen que estar acabadas en un carácter nulo.

El protocolo usará TCP. El servidor debe escuchar por omisión en el puerto 9999.

Se debe aplicar un timeout de 30 segundos.

*/








    
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