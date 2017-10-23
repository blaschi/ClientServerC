#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#define DEF_PORT "8081"
#define MAX_DATA_SIZE 100
#define LOCALHOST "127.0.0.1"
#define BUFFER_SIZE 100

void modeListen(int);
void modeSend(int);

int main(int argc, char *argv[]) {
   
    int addrinfoStatus;
    int socketClient;
    struct addrinfo *servinfo;
    struct addrinfo hints;
    char opcion = ' ';

    memset(&hints, 0, sizeof(hints));
    
    //Definir tipo de conexion
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((addrinfoStatus = getaddrinfo(LOCALHOST, DEF_PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addrinfoStatus));
        exit(1);
    }

    if ((socketClient = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1 ) {
        perror("Client: socket");
        exit(1);
    }
    
    if (connect(socketClient, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        close(socketClient);
        perror("Client: connect");
        exit(1);
    }
    printf("Coneccion establecida con el Servidor!");

    while(opcion != 'q') {
        printf("q - Salir \n");
        printf("l - Esperar mensaje del Server \n");
        printf("s - Enviar mensaje al Server \n");
        scanf("%c", &opcion);

        switch (opcion) {
            case 'l':
                modeListen(socketClient);
            break;
                
            case 's':
                modeSend(socketClient);
            break;
                
            default:
                if (opcion != 'q')
                    printf("Opcion incorrecta, intente de nuevo \n");
            break;
        }       
    }

    freeaddrinfo(servinfo);
    close(socketClient);

    return 0;
}

void modeListen(int socketClient) {
    char recvBuffer[BUFFER_SIZE];    
    //Leer mensaje recibido por parte del Server
    if( recv(socketClient, recvBuffer, MAX_DATA_SIZE-1, 0) < 0) {
        perror("Error al recibir mensaje");
        exit(EXIT_FAILURE);
    }

    //Mostrar mensaje
    recvBuffer[MAX_DATA_SIZE] = '\0';
    printf("Mensaje recibido: '%s'\n", recvBuffer);
    sleep(30);
}    

void modeSend(int socketClient) {

}