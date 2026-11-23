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
#define ARGS 6
#define MAX_CORES 13
#define ZOOM .75
#define FRAMES 50.0
int main(int argc, char* argv[]){
    pid_t first = getpid();
    if(argc == 2){
        if(!strcmp(argv[1], "-h")){ // print help
            printf("to run:\n ./mandelmovie <# cores>\n");
        }
        else{
            int processors = atoi(argv[1]);
            if(processors < 1){ //assure a valid processor # is entered
                printf("choose a processor value between 1 and %d.\n", MAX_CORES);
                exit(1);
            }
            double s = 4;
            mkdir("images", 0755);
            pid_t pid = 0;
            for(int i = 0; i < log2(processors); i++){
                pid |= (fork() == 0) << i;
            }
            if(pid < processors){ //only run the needed forks
                // int end = pid == processors-1? FRAMES : strt+FRAMES/processors;
                int i = pid;
                while(i < FRAMES){
                    s = pow(ZOOM, i*2);
                    char name[100];
                    sprintf(name, "images/image%d.jpeg", i);
                    run(-1.7706660190054566, -0.016332605287125934, s, 1000, 1000, 1000, name);

                    i+=processors;
                }
            }
            while(wait(NULL)>0){} //wait for everything to finish
            if(getpid() == first){//create mp4 from the parent
                system("ffmpeg -y -i images/image%d.jpeg mandel.mpg");
            }
        }
    }
    else{
        printf("enter 1 argument.\n");
        exit(1);
    }
}
