/**
 * author: korvan nameni
 * file: mandelmovie.c
 * description: create a movie of zooming in on a mandelbrot set
 * class: CPE-2600-111
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mandel.h"
#include <sys/stat.h>
#include <sys/wait.h>
#include <math.h>
#include <pthread.h>
#define ARGS 6
#define ZOOM .75
#define FRAMES 50.0
void create_movie(int processors, int threads);
int main(int argc, char* argv[]){
    char c;
    int processors = 1;
    int threads = 1;
    while((c = getopt(argc, argv, "p:t:h")) != -1){
        switch(c){
            case 'p':
                processors = atoi(optarg);
                break;
            case 't':
                threads = atoi(optarg);
                break;
            case 'h':
                printf("to run:\n ./mandelmovie -p <# cores> -t <# threads>\n");
                exit(0);
        }
    }
    create_movie(processors, threads);
}

void create_movie(int processors, int threads){
    pid_t first = getpid();
    if(processors < 1){ //assure a valid processor # is entered
        printf("choose a processor above 1.\n");
        exit(1);
    }
    if(threads < 1 || threads > 20){ //assure a valid processor # is entered
        printf("choose a processor value between 1 and 20.\n");
        exit(1);
    }
    double s = 4;
    mkdir("images", 0755);
    pid_t pid = 0;
    for(int i = 0; i < log2(processors); i++){
        pid |= (fork() == 0) << i;
    }
    if(pid < processors){ //only run the needed forks
        int i = pid;
        while(i < FRAMES){
            s = pow(ZOOM, i*2);
            char name[100];
            sprintf(name, "images/image%d.jpeg", i);
            run(-1.7706660190054566, -0.016332605287125934, s, 1000, 1000, 1000, name, threads);

            i+=processors;
        }
    }
    while(wait(NULL)>0){} //wait for everything to finish
    if(getpid() == first){//create mp4 from the parent
        system("ffmpeg -y -i images/image%d.jpeg mandel.mp4");
    }
}