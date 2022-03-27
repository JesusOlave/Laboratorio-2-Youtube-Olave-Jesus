#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

//Generamos nuevos hijos para el proceso principal
pid_t spawnChild(const char* program, char** arg_list)
{
    //Obtenemos el id del proceso hijo
    pid_t ch_pid = fork();
    // si el id del hijo es -1 hubo un  error y el programa se detiene
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (ch_pid == 0) {
        //En caso de no haber error ejecutamos el comando execve para ejecutar un programa o un comando
        execvp(program, arg_list);
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
        printf("spawned child with pid - %d\n", ch_pid);
        return ch_pid;
    }
}

int main(int argc, char *argv[]){

    //en caso de que no se cumpla con la cantidad de parametros necesarios el programa se cierra
    if(argc < 1){
        return 0;
    }

    //asignamos la url pasada por parametros a una variable char *
    char *urlYTB = argv[1];

    //en una instancia inicial la idea era habrir directamente el programa youtube dl
    system("youtube-dl");

    //asignamos el nombre del programa a un char para posteriormente pasarlo a la funcion spawnChild
    char *args[] = { "youtube-dl", NULL, NULL };

    //creamos un id para el hijo inicialmente vacio
    pid_t child;
    //creamos una variable para controlar el estado de la espera de id
    int wstatus;
    //usamos la funcion para generar un hijo y le pasamos los argumentos previamente instanciados
    child = spawnChild("youtube-dl", args);
    //vemos la instancia del id, en caso de que su estado sea -1 se termina con el programa
    if (waitpid(child, &wstatus, WUNTRACED | WCONTINUED) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    return 0;
}