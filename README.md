# lab11_multiprocessing_mandelbrot
#### This project is a demonstration of multiprocessing in image generation.
Program creates 50 mandelbrot images and processes them with a number of processors determined by the input. Task is completed using `fork()`.
Each image is also calculated individually with `pthread`.

To run (terminal):
1. `make`
2. `./mandelmovie -p <# processors> -t <# threads>`
note: the default is 20 cores and 1 thread.

The image output will be located in the folder `images`.

![alt text](graph.png)
As shown by the graph, there is an inverse logarithmic relationship between the number of processors used and the time it takes to complete 50 mandelbrot calculations.

![alt text](table.png)