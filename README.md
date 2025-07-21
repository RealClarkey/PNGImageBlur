# PNG Image Box Blur with Pthreads
This program applies a box blur filter to a PNG image using multithreading via the PThreads library. The blur is applied using a 3x3 kernel, averaging the RGB values of each pixel and its surrounding neighbours to produce a smooth output image. The workload is split evenly across multiple threads, which are specified at runtime.

# Features
- Reads and decodes PNG images using the lodepng library.
- Applies a 3x3 box blur filter to smooth the image.
- Handles corner, edge, and centre pixels with the appropriate averaging logic.
- Utilises dynamic memory allocation with malloc.
- Supports any number of threads, evenly distributing pixel rows per thread.
- Outputs a correctly blurred image as a new PNG file.

# Prerequisites
- GCC or compatible C compiler
- lodepng for PNG decoding/encoding
- PThreads library for multithreading

### How to Use

To compile and run the program, use the following commands in the terminal:

```bash
gcc PNGImageBlur.c lodepng.c -lpthread
./a.out [filename] [thread count]
