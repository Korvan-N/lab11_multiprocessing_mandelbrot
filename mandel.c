/**
 * author: korvan nameni
 * file: mandel.c
 * class: CPE-2600-111
 *  Based on example code found here:
 *  https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
 *
 *  Converted to use jpg instead of BMP and other minor changes
 */
#include <stdio.h>
#include "mandel.h"
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
//declare args so they don't need to be passed every compute_image call
imgRawImage *img;
double xmin;
double xmax;
double ymin;
double ymax;
int maximum;
int threads;
int hjump;
int run( double xcenter, double ycenter, double scale, int width, int height, int max, char* outfile, int threads)
{
	double xscale = scale;
	// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
	double yscale = scale / width * height;

	// Display the configuration of the image.
	printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile=%s\n",xcenter,ycenter,xscale,yscale,max,outfile);

	// Create a raw image of the appropriate size.
	img = initRawImage(width,height);

	// Fill it with a black
	setImageCOLOR(img,0);

	//globally declare variables reused in compute_image
	img = img;
	xmin = xcenter-xscale/2;         
    xmax = xcenter+xscale/2;         
	ymin = ycenter-yscale/2;         
	ymax = ycenter+yscale/2;         
    maximum = max;
	hjump = ceil(img->height*1.0/threads);
	
	int i = 0;
	//init threads
	pthread_t all_threads[threads]; 
	// Compute the Mandelbrot image
	while(i < threads){ //iterate 
		int tmp = i*hjump;
		int* hstart = malloc(sizeof(tmp));
		*hstart = tmp; //add height start to heap
		if(pthread_create(&all_threads[i], NULL, compute_image, hstart) != 0){ //gather threads
			perror("mandel pthread");
		}
		i++;
	}
	i = 0;
	while(i < threads){ //wait for all threads
		pthread_join(all_threads[i], NULL);
		i++;
	}
	
	// Save the image in the stated file.
	storeJpegImageFile(img,outfile);

	// free the mallocs
	freeRawImage(img);
	return 0;
}




/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void* compute_image(void* arg)
{
	int hstart = *(int*)(arg);
	int i,j;
	int width = img->width;
	int height = img->height;
	int hend = hstart+hjump < height ? hstart+hjump : height;
	// For every pixel in the image...

	for(j=hstart;j < hend;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax - xmin)/width;
			double y = ymin + j*(ymax - ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,maximum);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,maximum));
		}
	}
	free(arg); //free heap var
	return NULL;
}


/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}