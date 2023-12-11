#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <time.h>

void quitartxt(char *archivo){
    /*Funcion se encarga de quitarle la extesion .txt al archivo que esta leyendo
    para luego pasaralo todo a mayusculas y se enviado por un puntero a su variable 	 original*/    
    char *punto = strrchr(archivo, '.');
    if (punto && punto != archivo) {
        *punto = '\0';
    }

    int length = strlen(archivo);

    for (int i = 0; i < length; i++) {
        archivo[i] = toupper(archivo[i]); // Convertimos cada caracter a mayúsculas
    }}

void quitarespacios(char *linea){
    /*Funcion recibe una linea de la sopa de letras y se encarga de quitarle los espacios 
    y carcateres especiales que tenga*/	
    int fltIndex = 0;
    for (int i = 0; linea[i]; i++){
        if(!isspace(linea[i])){
            linea[fltIndex++] = linea[i];
            }
        }
    linea[fltIndex++]  = '\0';
}


int main() {

    //CREACIÓN CARPETAS.
    if (mkdir("CWD", 0777) == 0) {
        mkdir("CWD/Horizontal", 0777);
        mkdir("CWD/Vertical", 0777);
        mkdir("CWD/Horizontal/50x50", 0777);
        mkdir("CWD/Horizontal/100x100", 0777);
        mkdir("CWD/Horizontal/200x200", 0777);
        mkdir("CWD/Vertical/50x50", 0777);
        mkdir("CWD/Vertical/100x100", 0777);
        mkdir("CWD/Vertical/200x200", 0777);

        printf("Carpetas creada exitosamente.\n");
    } else {
        printf("No se pudo crear la carpeta o estas ya se encuentran creadas.\n");
    }

    DIR *dir = opendir("Casos");
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return 1;
    }

    char ubi[300];
    char pos[256];
    char destino[300];
    char fl[400];

    struct dirent *entry;

    clock_t inicio, fin;
    double tiempo_transcurrido;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Filtrar por archivos regulares
            char nombrearchivo[256];
            strcpy(nombrearchivo, entry->d_name);
            quitartxt(nombrearchivo);
            printf("------------------------\n");
            printf("Nombre de archivo: %s\n", entry->d_name);
            snprintf(ubi, sizeof(ubi), "Casos/%s", entry->d_name);
            FILE *file = fopen(ubi, "r");
            if (file == NULL) {
                perror("No se pudo abrir el archivo");
                continue;
            }
            fgets(pos, sizeof(pos), file);
            fgets(fl, sizeof(fl), file);
            quitarespacios(fl);
            int tamanio = strlen(fl);
            printf(" Tamaño de archivo: %d\n", tamanio);
            int x = 0;
            if ((pos[0] == 'h') || (pos[0] == 'H')) {
                snprintf(destino, sizeof(destino), "CWD/Horizontal/%dx%d/%s",tamanio,tamanio, entry->d_name);
                printf(" Orientacion: Horizontal\n");
                inicio = clock();
                while(x < (tamanio+1)){
                    char* resultado = strstr(fl,nombrearchivo); /*Aqui se cambia el codigo de busqueda de regex -> strstr */
                    if(resultado){
                        fin = clock();
                        printf(" ✔ Encontrado: %s\n", nombrearchivo);
                        x = tamanio+1;         
                    }
                    else{
                        fgets(fl, sizeof(fl), file); /*desconozco porque al hacer fgets solo una vez detecta una linea vacía, esto hace que en tamanio+1 solo lea la mitad*/ 
                        if(tamanio == 200){
                            fgets(fl, sizeof(fl), file);
                        }
                        quitarespacios(fl);
                        x++;
                    }
                }
            }
            if ((pos[0] == 'v') || (pos[0] == 'V')){
                snprintf(destino, sizeof(destino), "CWD/Vertical/%dx%d/%s", tamanio, tamanio, entry->d_name);
                printf(" Orientacion: Vertical\n"); 
                char matriz[tamanio+1][tamanio+1];
                while (x < tamanio){
                    int y = 0;
                    while (y < tamanio){
                        matriz[y][x] = fl[y];
                        y++;
                    }
                    matriz[y][x] = '\0';
                    if (tamanio == 200){
                        fgets(fl, sizeof(fl), file);
                        fgets(fl, sizeof(fl), file);    
                    }
                    else {
                        fgets(fl, sizeof(fl), file);
                    }  
                    quitarespacios(fl);    
                    x++;
                    
                }
                x= 0;
                inicio = clock();
                char fila[tamanio+1];
                for (int i = 0; i < tamanio; i++) {
                    fila[i] = matriz[x][i];
                    }
                while(x < (tamanio+1)){
                    char* resultado = strstr(fila,nombrearchivo); /*Aqui se cambia el codigo de busqueda de regex -> strstr */
                    if(resultado){
                        fin = clock();
                        printf(" ✔ Encontrado: %s\n", nombrearchivo);
                        x = tamanio+1;         
                    }
                    else{
                        for (int i = 0; i < tamanio; i++) {
                            fila[i] = matriz[x][i];
                            }
                    }
                    x++;
                }

            }
            
            tiempo_transcurrido = (double)(fin - inicio) / CLOCKS_PER_SEC;
            printf(" Tiempo transcurrido: %.6f segundos\n", tiempo_transcurrido);
            if (rename(ubi, destino) == 0) {
                printf(" Archivo movido exitosamente a %s.\n", destino);
            } else {
                perror("No se pudo mover el archivo");
            }
            fclose(file);

        }
    }

    // Cerrar el directorio
    closedir(dir);

    return 0;

}
