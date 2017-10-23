#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define DEF_MAX_CONNECTION 25
#define DEF_PORT "8081"
#define LOCALHOST "127.0.0.1"


void *threadHandler(void *);
void initializeServer(char);
void startMultiThread(int);
void startSelect(int);
