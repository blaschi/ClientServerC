#include "server.h"




int main(int argc, char *argv[]) {

    char arg;
    //Chequear cantidad de parametros
    if (argc == 2) {
        //Asignar parametro a variable
        arg = *(argv[1]);
        //Chequear parametro
        if ((arg == 't') || (arg == 's')) {
            //Pasar modo de operacion de servidor
            initializeServer(arg);
        }
        else {
            printf("Parametro incorrecto: t para threads o s para select \n");
        }
    }
    else {
        printf("Cantidad de parametros incorrecta \n");
    }
    
    return 0;
}