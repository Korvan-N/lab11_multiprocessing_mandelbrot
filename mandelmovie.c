/**
 * author: korvan nameni
 * file: mandelmovie.c
 * description: create a movie of zooming in on a mandelbrot set
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
#define MAX_CORES 12
#define ZOOM .75
#define FRAMES 50
void show_help();
int main(int argc, char* argv[]){
    pid_t first = getpid();
    if(argc == 2){
        if(!strcmp(argv[1], "-h")){
            show_help();
        }
        else{
            int processors = atoi(argv[1]);
            if(processors > MAX_CORES || processors < 1){
                printf("choose a processor value between 1 and %d.\n", MAX_CORES);
                exit(1);
            }
            double s = 4;
            mkdir("images", 0755);
            printf("%d frames\n", FRAMES);
            pid_t pid = 0;
            for(int i = 0; i < log2(processors); i++){
                pid_t me = fork();
                pid |= (me == 0) << i;
            }
            if(pid < processors){ //exit unneeded forks, 0 indexed
                int strt = pid*(FRAMES/processors);
                int end = pid == processors-1? FRAMES : strt+FRAMES/processors;
                for(int i = strt; i < end; i++){
                    s = pow(ZOOM, i*2);
                    char name[100];
                    sprintf(name, "images/image%d.jpeg", i);
                    run(-1.7706660190054566, -0.016332605287125934, s, 1000, 1000, 1000, name);
                }
            }
        }
    }
    while(wait(NULL)>0){}
    if(getpid() == first){
        system("ffmpeg -y -framerate 15 -i images/image%d.jpeg -c:v libx264 -pix_fmt yuv420p output.mp4");
    }
}
// Show help message
void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-f is the frame count");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}
