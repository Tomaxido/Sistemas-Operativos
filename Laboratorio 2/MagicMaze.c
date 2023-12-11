#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>

void consola(char *msg){
    /*
    Funcion que se encarga de mostrar por pantalla las acciones que puede realizar el jugador, y guardarla en una variable.
    */
    while(true){
        printf("--------------------------------------------\n");
        printf("Selecciona una accion:\n");
        printf("1) Moverse.\n");
        printf("2) Carta.\n");
        printf("3) Pasar.\n");
        printf("--------------------------------------------\n");
        printf("Ingrese opción: ");
        int op;
        scanf("%d", &op);
        int dir;
        int cant;
        if (op == 1){
            printf("Has seleccionado moverte.");
            while (true){
                printf("¿En qué dirección desea moverse?\n");
                printf("1) Arriba\n2) Abajo\n3) Izquierda\n4) Derecha\n");
                printf("Ingrese dirección: ");
                scanf("%d",&dir);
                if(dir >= 1 && dir <= 4){
                    while (true){
                        printf("Ingrese cantidad de casillas que desea moverse (Máximo 9): ");        
                        scanf("%d",&cant);
                        if (cant >= 0 && cant <= 9){
                            snprintf(msg, 4, "%d%d%d", op, dir, cant);
                            return;
                        }
                        printf("Opción incorrecta, intentelo nuevamente.\n");
                    }
                }
                printf("Opción incorrecta, intentelo nuevamente.\n");
            }
        }
        else if (op == 2) {
            printf("Haz utilizado tu carta! 🃏\n");
            snprintf(msg, 4, "%d%d%d", op, dir, cant);
            printf("--------------------------------------------\n");
            return;
        }
        else if (op == 3) {
            printf("Haz omitido tu turno\n");
            snprintf(msg, 4, "%d%d%d", op, dir, cant);
            printf("--------------------------------------------\n");
            return;
        }
        printf("Opción incorrecta, intentelo nuevamente.\n");
    }
}

void comando(char *msg, int i, char *resp){
    /*
        Funcion que se encargar de traducir la accion realizada por cada jugador para mostrarla por pantalla.
    */
    if(msg[0] == '1'){
        if(msg[1] == '1'){
            sprintf(resp, "El jugador %d ha decidido moverse %c hacia arriba.\n", i, msg[2]);
        }
        else if (msg[1] == '2'){
            sprintf(resp, "El jugador %d ha decidido moverse %c hacia abajo.\n", i, msg[2]);
        }
        else if (msg[1] == '3'){
            sprintf(resp, "El jugador %d ha decidido moverse %c a la izquierda.\n", i, msg[2]);
        }
        else{
            sprintf(resp, "El jugador %d ha decidido moverse %c a la derecha.\n", i, msg[2]);
        }
    }
    else if (msg[0] == '2') {
        sprintf(resp, "El jugador %d ha decidido usar su carta.\n",i);
    }
    else{
        sprintf(resp, "El jugador %d ha decidido pasar su turno.\n",i);
    }
}
int comprobar(int piso, char dir, int ubilab[][2], char mapagrande[13][13]){
    /*
    Funcion que se encarga de comprobar si en cierta direccion se encuentra conectado un laberinto o no.
    */
    if(dir == 'U'){
        if(mapagrande[ubilab[piso][0]-1][ubilab[piso][1]] != '-'){
            return 1;
        }
    }
    else if (dir == 'D'){
        if(mapagrande[ubilab[piso][0]+1][ubilab[piso][1]] != '-'){
            return 1;
        }
    }
    else if (dir == 'L'){
        if(mapagrande[ubilab[piso][0]][ubilab[piso][1]-1] != '-'){
            return 1;
        }
    }
    else if (dir == 'R'){
        if(mapagrande[ubilab[piso][0]][ubilab[piso][1]+1] != '-'){
            return 1;
        }
    }
    return 0;
}
void cambio_lab(int j, int posJ[][3], char dir, int ubilab[][2],char mapagrande[13][13]){
    /*
    Funcion que se encarga de mover al jugador de un piso a otro y ubicarlo bien en el nuevo laberinto que se encuentra segun la direccion por la que entre.
    */
    if((dir == 'U')){
        char x = mapagrande[ubilab[posJ[j][0]][0]-1][ubilab[posJ[j][0]][1]];
        posJ[j][0] = atoi(&(x));
        posJ[j][1] = 4;
        posJ[j][2] = 2;
    }
    else if((dir == 'D')){
        char x = mapagrande[ubilab[posJ[j][0]][0]+1][ubilab[posJ[j][0]][1]];
        posJ[j][0] = atoi(&(x));
        posJ[j][1] = 0;
        posJ[j][2] = 2;
    }
    else if((dir == 'L')){
        char x = mapagrande[ubilab[posJ[j][0]][0]][ubilab[posJ[j][0]][1]-1];
        posJ[j][0] = atoi(&(x));
        posJ[j][1] = 2;
        posJ[j][2] = 4;
    }
    else if((dir == 'R')){
        char x = mapagrande[ubilab[posJ[j][0]][0]][ubilab[posJ[j][0]][1]+1];
        posJ[j][0] = atoi(&(x));
        posJ[j][1] = 2;
        posJ[j][2] = 0;
    }
}

void accion_especial(int j,char casilla, int *turnos, int *camaras_activas, int ubi_tp[][3], int posJ[][3]){
    /*
    Funcion que se encarga de realizar los cambios correspondiente a la casilla especial por la que pasa el jugador.
    */
    if(casilla == 'c'){
        (*camaras_activas)--;
    }
    else if(casilla == '+'){
        if(*camaras_activas < 2){ *turnos = *turnos + 5;}
    }
    else if(casilla == '-'){
        *turnos = *turnos - 3;
    }
    else if (casilla == 't'){
        int piso = posJ[j][0];
        int destino = -1;
        do {
            srand(time(NULL));
            destino = (rand() % 7);
        } while(ubi_tp[destino][0] == piso || ubi_tp[destino][1] == 6 || ubi_tp[destino][2] == 6);
        posJ[j][1] = ubi_tp[destino][1];
        posJ[j][2] = ubi_tp[destino][2];
        posJ[j][0] = ubi_tp[destino][0];
    }
}

void movimiento(int j, char *msg, int posJ[][3], char mapa[][5][5], char *jugadores,int tesoroJencontrado[], int poscasilla[][2],int ubilab[][2], char mapagrande[13][13], int *turnos, int *camaras_activas, int ubi_tp[][3]){
    /*
    Funcion que se encarga del movimiento del jugador, comprobando si es que pasa por un tesoro, puerta, casilla b, especial, etc.
    se ve mucho codigo pero es que se va repitiendo para cada direccion jaja.
    */
    int piso = posJ[j][0];
    int x = posJ[j][1];
    int y = posJ[j][2];
    char player = jugadores[j];
    int z = 0;
    int mov = atoi(&msg[2]);
    int newpos;
    int tesoro = j+1;
    int cambio = 0;
    mapa[piso][x][y] = '0';
    if((x == 4 && y == 2) || (x == 2 && y == 0) || (x == 0 && y == 2) || (x == 2 && y == 4)){
        mapa[piso][x][y] = 'B';  
    }
    if (piso != 0 &&ubi_tp[piso-1][1] == x && ubi_tp[piso-1][2] == y){
        mapa[piso][x][y] = 't';  
    }
    if(msg[1]== '1'){ //arriba
        for (int i = 1; i<=mov; i++){ // se encarga de recorrer el camino que realizara el jugador, para ver si hay alguna casilla que lo limite.
            if(mapa[piso][x-i][y] == '/'){ // caso que se encuentre con una pared.
                mov = i;
                break;
            }
            else if(mapa[piso][x-i][y] == ((char)(tesoro + '0'))){ //comprobar si pasa por el tesoro correspondiente, si es asi actualiza parametros.
                printf("Tesoro obtenido.\n");
                tesoroJencontrado[j] = 1;
                poscasilla[piso][0] = 6;
                poscasilla[piso][1] = 6;
                mapa[piso][x-i][y] = '0';
            }
            else if(mapa[piso][x-i][y] == 'c' || mapa[piso][x-i][y] == '+'|| mapa[piso][x-i][y] == '-'|| mapa[piso][x-i][y] == 't'){ // este es el caso que encuentra una casilla especial
                accion_especial(j,mapa[piso][x-i][y],turnos,camaras_activas,ubi_tp,posJ);
                if(mapa[piso][x-i][y] != 't'){
                    mapa[piso][x-i][y]= '0';
                }
                else{
                    if(ubi_tp[8][0]>1){
                        piso = posJ[j][0];
                        x = posJ[j][1];
                        y = posJ[j][2];
                        mov = mov - i - 1;
                    }
                }
            }
            else if(cambio != 1 && (mapa[piso][x][y] == 'B' || mapa[piso][x-i][y] == 'B') && (x-i-1) < 0){// caso que pase por una B, comprueba si hay laberinto y lo cambia de laberinto si se cumple lo anterior.
                if(comprobar(piso,'U',ubilab,mapagrande) == 1){
                    cambio_lab(j,posJ,'U',ubilab,mapagrande);
                    if(mapa[piso][x][y] == 'B'){mov++;}                    
                    piso = posJ[j][0];
                    x = posJ[j][1];
                    y = posJ[j][2];
                    mov = mov - i - 1;
                    cambio = 1;
                }
                else{
                    mov = i;
                    break;
                }
            }
            else if(mapa[piso][x-i][y] == 'E'){
                mov = i;
                break;
            }
            
        }
        newpos = x-mov;
        if ((newpos<0)){
            newpos = 0;
        }
        while(true){ // si esque hay un jugador o muro movera al jugador una casilla anterior al movimiento que realizo, para posicionarlo bien.
            if(mapa[piso][newpos+z][y] == 'B' || mapa[piso][newpos+z][y] == 't'){ // reemplazar la casila B o t si es que el jugador queda encima de ella.
                mapa[piso][newpos+z][y] = player;
                posJ[j][1] = newpos+z;
                return;
            }
            else if(mapa[piso][newpos+z][y] != '0'){
                z++;
            }
            else{
                mapa[piso][newpos+z][y] = player;
                posJ[j][1] = newpos+z;
                return;
            }
        }
    }
    else if(msg[1]== '2'){ // abajo
        for (int i = 1; i<=mov; i++){
            if(mapa[piso][x+i][y] == '/'){
                mov = i;
                break;
            }
            else if(mapa[piso][x+i][y] == ((char)(tesoro + '0'))){
                tesoroJencontrado[j] = 1;
                poscasilla[piso][0] = 6;
                poscasilla[piso][1] = 6;
                mapa[piso][x+i][y] = '0';
            }
            else if(mapa[piso][x+i][y] == 'c' || mapa[piso][x+i][y] == '+'|| mapa[piso][x+i][y] == '-'|| mapa[piso][x+i][y] == 't'){
                accion_especial(j,mapa[piso][x+i][y],turnos,camaras_activas,ubi_tp,posJ);
                if(mapa[piso][x+i][y] != 't'){
                    mapa[piso][x+i][y]= '0';
                }
                else{
                    if(ubi_tp[8][0]>1){
                        piso = posJ[j][0];
                        x = posJ[j][1];
                        y = posJ[j][2];
                        mov = mov - i - 1;
                    }
                }
            }
            else if(cambio != 1 && (mapa[piso][x][y] == 'B'|| mapa[piso][x+i][y] == 'B' ) && (x+i+1) > 4){              
                if(comprobar(piso,'D',ubilab,mapagrande) == 1){
                    cambio_lab(j,posJ,'D',ubilab,mapagrande);
                    if(mapa[piso][x][y] == 'B'){mov++;}                    
                    piso = posJ[j][0];
                    x = posJ[j][1];
                    y = posJ[j][2];
                    mov = mov - i - 1;
                    cambio = 1;
                }
                else{
                    mov = i;
                    break;
                }
            }
            else if(mapa[piso][x+i][y] == 'E'){
                mov = i;
                break;
            }
        }
        newpos = x+mov;
        if ((newpos>4)){
            mov = 4;
        }          
        while(true){
            if(mapa[piso][newpos-z][y] == 'B' || mapa[piso][newpos-z][y] == 't'){
                mapa[piso][newpos-z][y] = player;
                posJ[j][1] = newpos-z;
                return;
            }
            else if(mapa[piso][newpos-z][y] != '0'){
                z++;
            }
            else{
                mapa[piso][newpos-z][y] = player;
                posJ[j][1] = newpos-z;
                return;
            }
        }
    }
    else if(msg[1]== '3'){ // izquierda
        for (int i = 1; i<=mov; i++){
            if(mapa[piso][x][y-i] == '/'){
                mov = i;
                break;
            }
            else if(mapa[piso][x][y-i] == ((char)(tesoro + '0'))){
                tesoroJencontrado[j] = 1;
                poscasilla[piso][0] = 6;
                poscasilla[piso][1] = 6;
                mapa[piso][x][y-i] = '0';
            }
            else if(mapa[piso][x][y-i] == 'c' || mapa[piso][x][y-i] == '+'|| mapa[piso][x][y-i] == '-'|| mapa[piso][x][y-i] == 't'){
                accion_especial(j,mapa[piso][x][y-i],turnos,camaras_activas,ubi_tp,posJ);
                if(mapa[piso][x][y-i] != 't'){
                    mapa[piso][x][y-i]= '0';
                }
                else{
                    if(ubi_tp[8][0]>1){
                        piso = posJ[j][0];
                        x = posJ[j][1];
                        y = posJ[j][2];
                        mov = mov - i - 1;
                    }
                }
            }
            else if(cambio != 1 && (mapa[piso][x][y] == 'B' || mapa[piso][x][y-i] == 'B') && (y-i-1) < 0){          
                if(comprobar(piso,'L',ubilab,mapagrande) == 1){
                    cambio_lab(j,posJ,'L',ubilab,mapagrande);
                    if(mapa[piso][x][y] == 'B'){mov++;}
                    piso = posJ[j][0];
                    x = posJ[j][1];
                    y = posJ[j][2];
                    mov = mov - i - 1;
                    cambio = 1;
                }
                else{
                    mov = i;
                    break;
                }
            }
            else if(mapa[piso][x][y-i] == 'E'){
                mov = i;
                break;
            }
        }
        newpos = y-mov;
        if ((newpos<0)){
            newpos = 0;
        }
        while(true){
            if(mapa[piso][x][newpos+z] == 'B' || mapa[piso][newpos-z][y] == 't'){
                mapa[piso][x][newpos+z] = player;
                posJ[j][2] = newpos+z;
                return;
            }
            else if(mapa[piso][x][newpos+z] != '0'){
                z++;
            }
            else{
                mapa[piso][x][newpos+z] = player;
                posJ[j][2] = newpos+z;
                return;
            }
        }
    }
    else if(msg[1]== '4'){ // derecha
        for (int i = 1; i<=mov; i++){
            if(mapa[piso][x][y+i] == '/'){
                mov = i;
                break;
            }
            else if(mapa[piso][x][y+i] == ((char)(tesoro + '0'))){
                tesoroJencontrado[j] = 1;
                poscasilla[piso][0] = 6;
                poscasilla[piso][1] = 6;
                mapa[piso][x][y+i] = '0';
            }
            else if(mapa[piso][x][y+i] == 'c' || mapa[piso][x][y+i] == '+'|| mapa[piso][x][y+i] == '-'|| mapa[piso][x][y+i] == 't'){
                accion_especial(j,mapa[piso][x][y+i],turnos,camaras_activas,ubi_tp,posJ);
                if(mapa[piso][x][y+i] != 't'){
                    mapa[piso][x][y+i]= '0';
                }
                else{
                    if(ubi_tp[8][0]>1){
                        piso = posJ[j][0];
                        x = posJ[j][1];
                        y = posJ[j][2];
                        mov = mov - i - 1;
                    }
                }
            }
            else if(cambio != 1 && (mapa[piso][x][y] == 'B' || mapa[piso][x][y+i] == 'B' ) && (y+i+1) > 4){
                if(comprobar(piso,'R',ubilab,mapagrande) == 1){
                    cambio_lab(j,posJ,'R',ubilab,mapagrande);
                    if(mapa[piso][x][y] == 'B'){mov++;}
                    piso = posJ[j][0];
                    x = posJ[j][1];
                    y = posJ[j][2];
                    mov = mov - i - 1;
                    cambio = 1;
                }
                else{
                    mov = i;
                    break;
                }
            }
            else if(mapa[piso][x][y+i] == 'E'){
                mov = i;
                break;
            }
        }
        newpos = y+mov;
        if ((newpos>4)){
            newpos = 4;
        }
        while(true){
            if(mapa[piso][x][newpos-z] == 'B' || mapa[piso][newpos-z][y] == 't'){
                mapa[piso][x][newpos-z] = player;
                posJ[j][2] = newpos-z;
                return;
            }
            else if(mapa[piso][x][newpos-z] != '0'){
                z++;
            }
            else{
                mapa[piso][x][newpos-z] = player;
                posJ[j][2] = newpos-z;
                return;
            }
        }
    }
}

void casillaespecial(char mapa[][5][5], int piso, int tesoroJenmapa[], int *totaltesoro,int poscasilla[][2], int *camaras_activas, int ubi_tp[][3]){
    /*
    Funcion que se encarga de añadir una casilla especial al nuevo laberinto que es insertado en el mapa.
    */
    srand(time(NULL));
    int aleatorio = rand() % 2;
    char opcion_elegida;
    if (*totaltesoro != 0 && aleatorio == 0) { // Tesoro
        int numeroAleatorio;
        do{
        numeroAleatorio = (rand() % 4) + 1;
        } while (tesoroJenmapa[numeroAleatorio-1] == 1);
        opcion_elegida = (char)(numeroAleatorio + '0');
        tesoroJenmapa[numeroAleatorio-1] = 1;
        (*totaltesoro)--; 
    } 
    else { //casilla especial
        char casillas[] = {'c', '+', '-', 't'};
        sleep(1);
        srand(time(NULL));
        int indice_aleatorio = rand() % 4;
        opcion_elegida = casillas[indice_aleatorio];
        if(opcion_elegida == 'c'){ (*camaras_activas)++;}
    }
    srand(time(NULL));
    int numero1;
    int numero2;
    do {
        numero1 = rand() % 5;
        numero2 = rand() % 5;
    } while (mapa[piso][numero1][numero2] != '0');
    poscasilla[piso][0] = numero1;
    poscasilla[piso][1] = numero2;
    if(opcion_elegida == 't'){ 
        ubi_tp[piso-1][1] = numero1;
        ubi_tp[piso-1][2] = numero2;
        ubi_tp[8][0]++;
    }
    mapa[piso][numero1][numero2] = opcion_elegida;
}
void linkear(int j, char mapagrande[13][13],int posJ[][3],int labuse[], int ubilab[][2], int *pisoadd){
    /*
    Funcion que se encarga de añadir un laberinto en el mapa segun la posicion de B.
    */
    int piso = posJ[j][0];
    int x = posJ[j][1];
    int y = posJ[j][2];
    int lab;
    srand(time(NULL));
    int numeroAleatorio;
    int recorrido = 0;
    if(x == 0 && y == 2){//{4,2} B se encuentra arriba y buscamos tableros con una B abajo.
        int tabs[] = {1,2,4,7,8};
        int usados[] = {0,0,0,0,0};
        do{
        numeroAleatorio = (rand() % 5);
        if(usados[numeroAleatorio] == 0){
            usados[numeroAleatorio] = 1;
            recorrido++;
            if (recorrido == 5){return;}
        }
        lab = tabs[numeroAleatorio];
        } while (labuse[lab] != 0);
        if(mapagrande[ubilab[piso][0]-1][ubilab[piso][1]] == '-'){
            ubilab[lab][0] = (ubilab[piso][0]-1);
            ubilab[lab][1] = (ubilab[piso][1]);
            mapagrande[ubilab[lab][0]][ubilab[lab][1]] = (char)(lab + '0');
            labuse[lab] = 1;
            (*pisoadd) = lab;
        }
    }
    
    else if (x == 4 && y == 2){ // {0,2} B se encuentra abajo y buscamos tableros con una B arriba.
        int tabs[] = {2,3,4,5,6};
        int usados[] = {0,0,0,0,0};
        do{
        numeroAleatorio = (rand() % 5);
        if(usados[numeroAleatorio] == 0){
            usados[numeroAleatorio] = 1;
            recorrido++;
            if (recorrido == 5){return;}
        }
        lab = tabs[numeroAleatorio];
        } while (labuse[lab] != 0);
        if(mapagrande[ubilab[piso][0]+1][ubilab[piso][1]] == '-'){
            ubilab[lab][0] = (ubilab[piso][0]+1);
            ubilab[lab][1] = (ubilab[piso][1]);
            mapagrande[ubilab[lab][0]][ubilab[lab][1]] = (char)(lab + '0');
            labuse[lab] = 1;
            (*pisoadd) = lab;
        }
    }
    else if (x == 2 && y == 0){ // {2,4} B se encuentra a la Izquierda y buscamos tableros con una B a la Derecha.
        int tabs[] = {1,2,3,5,8};
        int usados[] = {0,0,0,0,0};
        do{
        numeroAleatorio = (rand() % 5);
        if(usados[numeroAleatorio] == 0){
            usados[numeroAleatorio] = 1;
            recorrido++;
            if (recorrido == 5){return;}
        }
        lab = tabs[numeroAleatorio];
        } while (labuse[lab] != 0);
        if(mapagrande[ubilab[piso][0]][ubilab[piso][1]-1] == '-'){
            ubilab[lab][0] = (ubilab[piso][0]);
            ubilab[lab][1] = (ubilab[piso][1]-1);
            mapagrande[ubilab[lab][0]][ubilab[lab][1]] = (char)(lab + '0');
            labuse[lab] = 1;
            (*pisoadd) = lab;
        }
    }
    else { // {2,0} B se encuentra a la Derecha y buscamos tableros con una B a la Izquierda.
        int tabs[] = {1,3,4,6,7};
        int usados[] = {0,0,0,0,0};        
        do{
        numeroAleatorio = (rand() % 5);
        lab = tabs[numeroAleatorio];
        if(usados[numeroAleatorio] == 0){
            usados[numeroAleatorio] = 1;
            recorrido++;
            if (recorrido == 5){return;}
        }
        recorrido++;
        if (recorrido == 6){return;}
        } while (labuse[lab] != 0);
        if(mapagrande[ubilab[piso][0]][ubilab[piso][1]+1] == '-'){
            ubilab[lab][0] = (ubilab[piso][0]);
            ubilab[lab][1] = (ubilab[piso][1]+1);
            mapagrande[ubilab[lab][0]][ubilab[lab][1]] = (char)(lab + '0');
            labuse[lab] = 1;
            (*pisoadd) = lab;
        }
    }
    return;
}
void useCard(int j,char card, int posE[][3], int posB[][2], int posJ[][3],char matrices[][5][5], char mapagrande[13][13], int labuse[], int ubilab[][2],int tesoroJenmapa[], int *totaltesoro,int poscasilla[][2], int *camaras_activas, int ubi_tp[][3]){
    /*
    Funcion que se encarga de realizar la accion de la carta que utiliza el jugador, comprueba si la casilla esta encima o alrededor de el, si es asi redirecciona a las funciones
    correspondiente, caso contrario no hace nada.
    */
    int piso = posJ[j][0];
    int x = posJ[j][1];
    int y = posJ[j][2];
    if (card == 'B'){
        int pisoadd = -1;
        for (int i = 0; i < 4; i++){
            if (x == posB[i][0] && y == posB[i][1]){
                linkear(j,mapagrande,posJ,labuse,ubilab,&pisoadd);
                if (pisoadd >= 0) {casillaespecial(matrices,pisoadd,tesoroJenmapa,totaltesoro,poscasilla,camaras_activas,ubi_tp);}
            }
            else if(x+1 == posB[i][0] && y == posB[i][1]){
                posJ[j][1]++;
                linkear(j,mapagrande,posJ,labuse,ubilab,&pisoadd);
                if (pisoadd >= 0) {casillaespecial(matrices,pisoadd,tesoroJenmapa,totaltesoro,poscasilla,camaras_activas,ubi_tp);}
                posJ[j][1]--;
            }
            else if(x-1 == posB[i][0] && y == posB[i][1]){
                posJ[j][1]--;
                linkear(j,mapagrande,posJ,labuse,ubilab,&pisoadd);
                if (pisoadd >= 0) {casillaespecial(matrices,pisoadd,tesoroJenmapa,totaltesoro,poscasilla,camaras_activas,ubi_tp);}
                posJ[j][1]++;
            }
            else if(x == posB[i][0] && y+1== posB[i][1]){
                posJ[j][2]++;
                linkear(j,mapagrande,posJ,labuse,ubilab,&pisoadd);
                if (pisoadd >= 0) {casillaespecial(matrices,pisoadd,tesoroJenmapa,totaltesoro,poscasilla,camaras_activas,ubi_tp);}
                posJ[j][2]--;
            }
            else if(x == posB[i][0] && y-1 == posB[i][1]){
                posJ[j][2]--; 
                linkear(j,mapagrande,posJ,labuse,ubilab,&pisoadd);
                if (pisoadd >= 0) {casillaespecial(matrices,pisoadd,tesoroJenmapa,totaltesoro,poscasilla,camaras_activas,ubi_tp);}
                posJ[j][2]++;
            }
        }
    }
    else{
        for(int i = 0; i< 5; i++){
            if (piso == posE[i][0] && x == posE[i][1] && y == posE[i][2]){
                matrices[posE[i][0]][posE[i][1]][posE[i][2]] = '0';
                posE[i][0] = 9;
                posE[i][1] = 6;
                posE[i][2] = 6;
            }
            else if(piso == posE[i][0] && x+1 == posE[i][1] && y == posE[i][2]){
                matrices[posE[i][0]][posE[i][1]][posE[i][2]] = '0';
                posE[i][0] = 9;
                posE[i][1] = 6;
                posE[i][2] = 6;              
            }
            else if(piso == posE[i][0] && x-1 == posE[i][1] && y == posE[i][2]){
                matrices[posE[i][0]][posE[i][1]][posE[i][2]] = '0';
                posE[i][0] = 9;
                posE[i][1] = 6;
                posE[i][2] = 6;
            }
            else if(piso == posE[i][0] && x == posE[i][1] && y+1 == posE[i][2]){
                matrices[posE[i][0]][posE[i][1]][posE[i][2]] = '0';
                posE[i][0] = 9;
                posE[i][1] = 6;
                posE[i][2] = 6;
            }
            else if(piso == posE[i][0] && x == posE[i][1] && y-1 == posE[i][2]){
                matrices[posE[i][0]][posE[i][1]][posE[i][2]] = '0';
                posE[i][0] = 9;
                posE[i][1] = 6;
                posE[i][2] = 6;
            }
        }
    }
    return;
}

void assign_card(char cartas_asignadas[]){
    /*
    Funcion que se encarga de asignar las cartas de manera aleatoria a cada jugador.
    */
    srand(time(NULL));
    char cartas[] = {'B', 'P', 'B', 'P'};
    int numeroAleatorio;
    int playerAleatorio;
    do{
        numeroAleatorio = (rand() % 4);
        playerAleatorio = (rand() % 4);
        if(cartas_asignadas[playerAleatorio] == '-'){
            cartas_asignadas[playerAleatorio] = cartas[numeroAleatorio];
            cartas[numeroAleatorio] = '-';
        }
    }while(cartas[0] != '-' || cartas[1] != '-' || cartas[2] != '-' || cartas[3] != '-');
}

int main() {
    // Nombres de los archivos y variables
    char *nombres_archivos[] = {"./tableros/Inicio.txt", "./tableros/tablero1.txt", "./tableros/tablero2.txt", "./tableros/tablero3.txt", "./tableros/tablero4.txt", "./tableros/tablero5.txt", "./tableros/tablero6.txt", "./tableros/tablero7.txt", "./tableros/tablero8.txt"};
    char cartas_asignadas[] = {'-','-','-','-'};
    char jugadores[] = {'F','S','T','X'};
    int labuse[] = {1,0,0,0,0,0,0,0,0};
    int ubilab[][2] = {{6,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    int posJ[][3] = {{0,1,1}, {0,1,3}, {0,3,1}, {0,3,3}};
    int tesoroJencontrado[] = {0,0,0,0};
    int tesoroJenmapa[] = {0,0,0,0};
    int totaltesoro = 4;
    int camaras_activas = 0;
    int ubi_tp[][3] = {{1,6,6}, {2,6,6}, {3,6,6}, {4,6,6}, {5,6,6}, {6,6,6}, {7,6,6}, {8,6,6},{0,0,0}};
    int poscasilla[][2] = {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    int posE[][3] = {{1,2,3}, {3,1,2}, {5,1,2}, {6,2,1}, {8,2,3}}; // {piso, x, y}
    int posB[][2] = {{0,2},{4,2},{2,0},{2,4}};
    char matrices[9][5][5];
    char mapagrande[13][13];
    int suma;
    int inicio;
    int victoria;
    assign_card(cartas_asignadas);

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            mapagrande[i][j] = '-';
        }
    }

    // Cambiar la posición [5][5] a "0"
    mapagrande[ubilab[0][0]][ubilab[0][1]] = '0';

    // Imprimir la matriz
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 13; j++) {
            printf("%c ", mapagrande[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < 9; i++) {
        FILE *archivo = fopen(nombres_archivos[i], "r");
        if (archivo == NULL) {
            printf("No se pudo abrir el archivo %s.\n", nombres_archivos[i]);
            return 1;
        }
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                fscanf(archivo, " %c", &matrices[i][j][k]);
            }
        }
        fclose(archivo);
    }
    
    // Imprime todas las matrices
    for (int i = 0; i < 9; i++) {
        if( labuse[i] == 1){
            printf("Laberinto %d:\n", i);
            for (int j = 0; j < 5; j++) {
                for (int k = 0; k < 5; k++) {
                    printf("%c ", matrices[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }   
    int turnos = 15;
    for (int rep = 0; rep < turnos; rep++) {
        printf("\nRONDA %d\n",rep+1);
        printf("*TURNOS RESTANTES: %d\n", turnos-rep-1);
        printf("*Camaras activas: %d\n", camaras_activas);
        
        for(int i = 0; i<4; i++){
            if(cartas_asignadas[i] == 'B'){
                printf(" -Carta jugador %d: Buscar\n",i+1);
            }
            else{
                printf(" -Carta jugador %d: Puerta\n",i+1);
            }
        }
        int fd[2]; 
        int fdPB12[2];
        int fdPB13[2];
        int fdPB14[2];
        pipe(fd);  
        pipe(fdPB12);
        pipe(fdPB13);
        pipe(fdPB14);
        
        int num_hijos = 4;

        for (int i = 0; i < num_hijos; i++) {

            pid_t pid = fork();
            if (pid == 0) {
                if(i==0){
                    close(fd[0]);

                    char mensaje[100];
                    printf("Turno jugador 1:\n");
                    consola(mensaje);
                    write(fd[1], mensaje, sizeof(mensaje)); 
                    close(fd[1]); 

                    printf("Turno jugador 2:\n");
                    consola(mensaje);
                    write(fdPB12[1], mensaje, sizeof(mensaje));

                    printf("Turno jugador 3:\n");
                    consola(mensaje);
                    write(fdPB13[1], mensaje, sizeof(mensaje));

                    printf("Turno jugador 4:\n");
                    consola(mensaje);
                    write(fdPB14[1], mensaje, sizeof(mensaje));
                    exit(0);
                }
                else{
                    close(fd[0]);
                    char mensaje[100];
                    if (i == 1) {
                        read(fdPB12[0], mensaje, 100);
                        write(fd[1], mensaje,100);
                    }
                    else if (i == 2){
                        read(fdPB13[0], mensaje, 100);
                        write(fd[1], mensaje, 100);
                    }
                    else{
                        read(fdPB14[0], mensaje, 100);
                        write(fd[1], mensaje, 100);
                    }

                    close(fd[1]);
                    exit(0);
                }
            } else if (pid < 0) {
                perror("Error al crear el hijo");
                exit(1);
            }
        }
        close(fd[1]);
        for (int i = 0; i < num_hijos; i++) {
            write(fd[0], "Listo para escribir", sizeof("Listo para escribir"));
        }

        // Leer y mostrar los mensajes recibidos del pipe
        char buffer[100];
        char msg[100];
        int pj = 0;
        int hijos_activos = num_hijos;
        for (int i = 0; i < num_hijos; i++) {
            wait(NULL);
        }
        printf("Movimientos:\n");
        while (hijos_activos > 0) {
            for (int i = 0; i < num_hijos; i++) {
            wait(NULL);
            }

            if (read(fd[0], buffer, sizeof(buffer)) <= 0) {
                hijos_activos--;
            } else {
                comando(buffer,pj+1,msg);
                printf("-%s", msg);
                if(buffer[0] == '1'){
                    movimiento(pj,buffer,posJ,matrices,jugadores,tesoroJencontrado,poscasilla,ubilab,mapagrande,&turnos,&camaras_activas, ubi_tp);
                    }
                else if (buffer[0] == '2'){
                    useCard(pj,cartas_asignadas[pj],posE,posB,posJ,matrices,mapagrande,labuse,ubilab,tesoroJenmapa,&totaltesoro,poscasilla,&camaras_activas, ubi_tp);
                }
                pj++;
            }
        }
        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 13; j++) {
                printf("%c ", mapagrande[i][j]);
            }
            printf("\n");
        }   
        printf("\n");
        for (int i = 0; i < 9; i++) {
            if( labuse[i] == 1){
                printf("Laberinto %d:\n", i);
                for (int j = 0; j < 5; j++) {
                    for (int k = 0; k < 5; k++) {
                        printf("%c ", matrices[i][j][k]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        }     
        printf("Tesoros por descubrir: %d\n",totaltesoro);
        printf("Nuevas Posiciones:\n");
        for (int j = 0; j < 4; j++) {
            printf("Jugador %d:\n",j+1);
            printf(" -Laberinto: %d\n",posJ[j][0]);
            printf(" -Pos X: %d\n",posJ[j][1]);
            printf(" -Pos Y: %d\n",posJ[j][2]);
            if (tesoroJenmapa[j] == 0){
                printf(" -Tesoro no se ha generado.\n");
            }
            else{
                printf(" -Tesoro ya fue generado.\n");
            }
            if (tesoroJencontrado[j] == 0){
                printf(" -Tesoro no se ha encontrado.\n");
            }
            else{
                printf(" -Tesoro ya fue encontrado.\n");
            }

        }
        close(fd[0]); // Cerrar el extremo de lectura del pipe en el padre

        // Esperar a que todos los hijos terminen
        for (int i = 0; i < num_hijos; i++) {
            wait(NULL);
        }
        printf("\nPulse enter para continuar...");
        char continuar = getchar(); 
        continuar = ' ';
        
        printf("%c",continuar);
        suma = 0;
        victoria = 0;
        inicio = 0;
        for(int i = 0; i<4 ;i++){
            suma = suma + tesoroJencontrado[i];
            if(posJ[i][0] == 0){inicio++;}
        }
        if (suma == 4 && inicio == 4){
            victoria = 1;               
            break;
        }
        suma = 0;
        for(int i = 0; i<9 ; i++){
            suma = suma + labuse[i];
        }
        if(totaltesoro > 0 && suma == 9){
            break;
        }
    }
    if (victoria == 1) {
        printf("Todos los jugadores han conseguido su tesoro! Y han regresados al incio.\n");
        printf("¡¡VICTORIA!!\n");
    }else{
        printf("No han cumplido con los objetivos...\n");
        printf("¡¡DERROTA!!");
    }
    return 0;
}