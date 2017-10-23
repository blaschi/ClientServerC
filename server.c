#include "server.h"

    
    void initializeServer(char arg) {
        
        int addrinfoStatus, listenStatus;
        int socketServer;
        struct addrinfo *servinfo;
        struct addrinfo hints;        

        //Inicializar estructura
        memset(&hints, 0, sizeof(hints));

        //Definir tipo de conexion
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        //Verificar errores
        if ((addrinfoStatus = getaddrinfo(LOCALHOST, DEF_PORT, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(addrinfoStatus));
            exit(EXIT_FAILURE);
        }

        socketServer = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
        
        if (socketServer == -1) {
            perror("Server: socket");
            exit(EXIT_FAILURE);
        }
        //Bindear socket
        bind(socketServer, servinfo->ai_addr, servinfo->ai_addrlen);
        
        
        //Escuchar en puerto
        listenStatus = listen(socketServer, DEF_MAX_CONNECTION);
        
        if (listenStatus == -1) {
            close(socketServer);
            perror("Client: listen fallo");
            exit(EXIT_FAILURE);
        }

        
        //Implementar modo de operacion del Server        
        switch (arg) {
            case 't':
                startMultiThread(socketServer);
            break;

            case 's':
                startSelect(socketServer);
            break;

            default:
                printf("Error: parametro desconocido");
                exit(EXIT_FAILURE);
        }

        //Cerrar las conexiones
        close(socketServer);
        
        //Liberar recursos
        freeaddrinfo(servinfo);
    }
    
    //Implementar multi-threading para servidor
    void startMultiThread(int socketServer) {
        while(1) {

            int socketClient;
            socklen_t addrSize;
            struct sockaddr_storage addrClient;
            
            //Aceptar conexion y crear un nuevo thread por cada cliente
            addrSize = sizeof(addrClient);
            socketClient = accept(socketServer, (struct sockaddr*)&addrClient, &addrSize);
                
            if (socketClient == -1) {
                perror("Client: accept fallo");
                exit(EXIT_FAILURE);
            }

            pthread_t newThread;
            pthread_attr_t newThreadAttr;
            int *newSocketClient = malloc(sizeof(int));
            *newSocketClient = socketClient;
        
            pthread_attr_init(&newThreadAttr);
            pthread_attr_setdetachstate(&newThreadAttr, PTHREAD_CREATE_DETACHED);
            pthread_attr_setschedpolicy(&newThreadAttr, SCHED_FIFO);    
    
            if(pthread_create(&newThread, &newThreadAttr, threadHandler, newSocketClient) < 0) {
                perror("No se pudo crear el thread");
                exit(EXIT_FAILURE);
            }

        }
    }

    //Implementar modo sincronico Select
    void startSelect(int socketServer) {

        struct timeval tv;
        socklen_t addrSize;
        struct sockaddr sockSelectAddrClient;        
        
        int socketClient;

        fd_set readFds;
        fd_set tempReadFds;
        fd_set writeFds;
        fd_set exceptionFds;

        int fdMax = socketServer;

        tv.tv_sec = 2;
        tv.tv_usec = 500000;

        FD_ZERO(&readFds);
        FD_SET(socketServer, &readFds);

        while(1) {
            tempReadFds = readFds;

            int retSelect = select(fdMax+1, &readFds, NULL, NULL, NULL);//&tv);
            printf("Select return: %d \n", retSelect);

            if (retSelect == -1) {
                perror("Error: Select");
            }

            int i;
            for(i = 0; i <= fdMax; i++) {

                if (FD_ISSET(i, &tempReadFds)) {

                    printf("FD %d OK \n", i);

                    if (i == socketServer) {

                        socketClient = accept(socketServer, &sockSelectAddrClient, &addrSize);

                        FD_SET(socketClient, &readFds);

                        if(socketClient > fdMax) {
                            fdMax = socketClient;
                        }
                    }
                    else {
                        char *message;
                        message = "Hola, se ha establecido una nueva conexion con SELECT";                            
                            
                        write(i, message, strlen(message));
                        /*
                        if (retWrite == 0) {
                            FD_CLR(i, &readFds);
                            continue;
                        }
                        */
                        send(i, message, strlen(message), 0);
                            
                    }
                }
                else {
                    printf("%d, \n", i);
                }
            }
        }
    }        
    
    //Funcion del thread
    void *threadHandler(void *threadParams) {
        int socketDesc = *(int*)threadParams;
        int readSize;
        char *message, clientMessage[100];

        message = "Hola, se ha establecido una nueva conexion con threads";
        write(socketDesc, message, strlen(message));

        free(threadParams);

        return NULL;
    }
