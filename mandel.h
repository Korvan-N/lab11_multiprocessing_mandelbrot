/**
 * author: korvan nameni
 * file: mandel.h
 * class: CPE-2600-111
 */
#ifndef MANDEL_H
#define MANDEL_H
#include "jpegrw.h"
int iteration_to_color( int i, int max );
int iterations_at_point( double x, double y, int max );
void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
int run( double xcenter, double ycenter, double scale, int width, int height, int max, char* outfile);
#endif //MANDEL_H