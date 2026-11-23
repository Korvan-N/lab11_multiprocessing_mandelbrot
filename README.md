# lab11_multiprocessing_mandelbrot
#### This project is a demonstration of multiprocessing in image generation.
Program creates 50 mandelbrot images and processes them with a number of cores determined by the input. Task is completed using `fork()`

To run (terminal):
1. `make`
2. `./mandelmovie <# cores>`

The image output will be located in the folder `images`.

![alt text](graph.png)
As shown by the graph, there is an inverse logarithmic relationship between the number of processors used and the time it takes to complete 50 mandelbrot calculations.
![alt text](table.png)