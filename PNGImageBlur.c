#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lodepng.h"

// To use this file use the below commands in terminal:
// task4.c lodepng.c -pthread
// ./a.out [filename] [thread count]


// Initialising the struct
struct ThreadData {
    pthread_mutex_t mutex;
    unsigned char **image2D;
    unsigned char **newImage2D;
    int startOfRow;
    int endOfRow;
    int width;
    int height;
};

void* imageProcessing(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    
    pthread_mutex_lock(&data->mutex); //Ensure the mutex is locked before accessing the critical section.

    for (int row = data->startOfRow; row < data->endOfRow; row++) {
        for (int col = 0; col < data->width * 4; col += 4) {

    		// Checking all 4 corners of image pixels (Top left, top right, bottom right, bottom left).
            // Compute the average RGB values based on neighboring pixels.
    		if(row == 0 && col ==0){ //Top left corner pixel.
  				int newR = (data->image2D[row][col] + data->image2D[row][col+4] + data->image2D[row+1][col] + data->image2D[row+1][col+4])/4;
  				int newG = (data->image2D[row][col+1] + data->image2D[row][col+5] + data->image2D[row+1][col+1] + data->image2D[row+1][col+5])/4;
  				int newB = (data->image2D[row][col+2] + data->image2D[row][col+6] + data->image2D[row+1][col+2] + data->image2D[row+1][col+6])/4;
				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];

    		}else if (row == 0 && col == data->width * 4 - 4){ // Top right corner pixel.
    			int newR = (data->image2D[row][col] + data->image2D[row][col-4] + data->image2D[row+1][col] + data->image2D[row+1][col-4])/4;
    			int newG = (data->image2D[row][col+1] + data->image2D[row][col-3] + data->image2D[row+1][col+1] + data->image2D[row+1][col-3])/4;
    			int newB = (data->image2D[row][col+2] + data->image2D[row][col -2] + data->image2D[row+1][col+2] + data->image2D[row+1][col-2])/4;
				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];

    		}else if (row == data->height - 1 && col == data->width * 4 - 4){ // Bottom right corner pixel.
                int newR = (data->image2D[row][col] + data->image2D[row][col-4] + data->image2D[row-1][col] + data->image2D[row-1][col-4])/4;
                int newG = (data->image2D[row][col+1] + data->image2D[row][col-3] + data->image2D[row-1][col+1] + data->image2D[row-1][col-3])/4;
                int newB = (data->image2D[row][col+2] + data->image2D[row][col-2] + data->image2D[row-1][col+2] + data->image2D[row-1][col-2])/4;
                data->newImage2D[row][col] = newR;
                data->newImage2D[row][col+1] = newG;
                data->newImage2D[row][col+2] = newB;
                data->newImage2D[row][col+3] = data->image2D[row][col+3];

            }else if (row == data->height - 1 && col == 0){ // Bottom left corner pixel.
                int newR = (data->image2D[row][col] + data->image2D[row][col+4] + data->image2D[row-1][col] + data->image2D[row-1][col+4])/4;
                int newG = (data->image2D[row][col+1] + data->image2D[row][col+5] + data->image2D[row-1][col+1] + data->image2D[row-1][col+5])/4;
                int newB = (data->image2D[row][col+2] + data->image2D[row][col+6] + data->image2D[row-1][col+2] + data->image2D[row-1][col+6])/4;
                data->newImage2D[row][col] = newR;
                data->newImage2D[row][col+1] = newG;
                data->newImage2D[row][col+2] = newB;
                data->newImage2D[row][col+3] = data->image2D[row][col+3];

            }

    		// Checking for pixels on edge of the image (array).
            // Calculate the average value based on the pixel itself and its immediate neighbors (left, right, underneath, diagonal down left, diagonal down right) divided by 6.
    		else if(row == 0){ // Top side edge
    			int newR = (data->image2D[row][col] + data->image2D[row][col-4] + data->image2D[row][col+4] + data->image2D[row+1][col] + data->image2D[row+1][col-4] + data->image2D[row+1][col+4])/6;
    			int newG = (data->image2D[row][col+1] + data->image2D[row][col-3] + data->image2D[row][col+5] + data->image2D[row+1][col+1] + data->image2D[row+1][col-3] + data->image2D[row+1][col+5])/6;
    			int newB = (data->image2D[row][col+2] + data->image2D[row][col-2] + data->image2D[row][col+6] + data->image2D[row+1][col+2] + data->image2D[row+1][col-2] + data->image2D[row+1][col+6])/6;
  				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];

    		}else if (row == data->height - 1){ // Bottom side edge
    			int newR = (data->image2D[row][col] + data->image2D[row][col-4] + data->image2D[row][col+4] + data->image2D[row-1][col] + data->image2D[row-1][col-4] + data->image2D[row-1][col+4])/6;
    			int newG = (data->image2D[row][col+1] + data->image2D[row][col-3] + data->image2D[row][col+5] + data->image2D[row-1][col+1] + data->image2D[row-1][col-3] + data->image2D[row-1][col+5])/6;
    			int newB = (data->image2D[row][col+2] + data->image2D[row][col-2] + data->image2D[row][col+6] + data->image2D[row-1][col+2] + data->image2D[row-1][col-2] + data->image2D[row-1][col+6])/6;
  				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];

    		}else if (col == 0){ // Left side edge
    			int newR = (data->image2D[row][col] + data->image2D[row][col+4] + data->image2D[row-1][col] + data->image2D[row+1][col] + data->image2D[row-1][col+4] + data->image2D[row+1][col+4])/6;
    			int newG = (data->image2D[row][col+1] + data->image2D[row][col+5] + data->image2D[row-1][col+1] + data->image2D[row+1][col+1] + data->image2D[row-1][col+5] + data->image2D[row+1][col+5])/6;
    			int newB = (data->image2D[row][col+2] + data->image2D[row][col+6] + data->image2D[row-1][col+2] + data->image2D[row+1][col+2] + data->image2D[row-1][col+6] + data->image2D[row+1][col+6])/6;
				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];

    		}else if(col == data->width * 4 - 4){ // Right side edge
    			int newR = (data->image2D[row][col] + data->image2D[row][col-4] + data->newImage2D[row-1][col] + data->image2D[row+1][col] + data->image2D[row-1][col-4] + data->image2D[row+1][col-4])/6;
    			int newG = (data->image2D[row][col+1] + data->image2D[row][col-3] + data->newImage2D[row-1][col+1] + data->image2D[row+1][col+1] + data->image2D[row-1][col-3] + data->image2D[row+1][col-3])/6;
    			int newB = (data->image2D[row][col+2] + data->image2D[row][col-2] + data->newImage2D[row-1][col+2] + data->image2D[row+1][col+2] + data->image2D[row-1][col-2] + data->image2D[row+1][col-2])/6;
				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];


    		// If it's not a corner or edge, it's a central pixel. A central pixel will have 8 neighbouring pixels.
            // Process 8 neighbouring pixels and divide by 9 (including itself) to achieve an average.
    		}else{ // Central pixel
    			int newR = (data->image2D[row][col] + data->image2D[row][col-4] + data->image2D[row][col+4] + data->image2D[row-1][col] + data->image2D[row+1][col] + data->image2D[row-1][col-4] + data->image2D[row-1][col+4] + data->image2D[row+1][col-4] + data->image2D[row+1][col+4])/9;
    			int newG = (data->image2D[row][col+1] + data->image2D[row][col-3] + data->image2D[row][col+5] + data->image2D[row-1][col+1] + data->image2D[row+1][col+1] + data->image2D[row-1][col-3] + data->image2D[row-1][col+5] + data->image2D[row+1][col-3] + data->image2D[row+1][col+5])/9;
    			int newB = (data->image2D[row][col+2] + data->image2D[row][col-2] + data->image2D[row][col+6] + data->image2D[row-1][col+2] + data->image2D[row+1][col+2] + data->image2D[row-1][col-2] + data->image2D[row-1][col+6] + data->image2D[row+1][col-2] + data->image2D[row+1][col+6])/9;
				data->newImage2D[row][col] = newR;
  				data->newImage2D[row][col+1] = newG;
  				data->newImage2D[row][col+2] = newB;
  				data->newImage2D[row][col+3] = data->image2D[row][col+3];

    		}

		}

	}
	pthread_mutex_unlock(&data->mutex); // Unlock the mutex to signal the end of the critical section.
    pthread_exit(NULL);
}

//Start of main function.
void  main(int argc, char** argv) {
    const char* filename = argv[1];
    unsigned char* image1D;
    unsigned int width;
    unsigned int height;

    int threadCounts = atoi(argv[2]);// Thread count from command line argument.

    lodepng_decode32_file(&image1D, &width, &height, filename); // Convert the image into a 1D array

    // Allocate memory for the 2D array representing the original image
    unsigned char** image2D = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for (int i = 0; i < height; i++) {
        image2D[i] = (unsigned char*)malloc(width * 4 * sizeof(unsigned char));
    }

    for (int row = 0; row < height; row++) { //For loop converting 1d array from decode function into 2D array.
        for (int col = 0; col < width * 4; col++) {
            image2D[row][col] = image1D[row * width * 4 + col];
        }
    }

    free(image1D); //Free memory storing 1D array.

    // Dynamically allocate memory for the 2D array for the new image
    unsigned char** newImage2D = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for (int i = 0; i < height; i++) {
        newImage2D[i] = (unsigned char*)malloc(width * 4 * sizeof(unsigned char));
    }

    // Initialising pthreads
    pthread_t threadIDs[threadCounts];
    struct ThreadData threadData[threadCounts];
    int rowsPerThread = height / threadCounts; // Workload assigned to each thread
    int remainder = height % threadCounts; // Find the remainder for adjustment


    int startOfRow = 0;
    for (int i = 0; i < threadCounts; i++) {
        // Adding another row if there is a remainder.
        int extraRow = 0;
        if(i < remainder){
            extraRow = 1;
            }
        int endOfRow = startOfRow + rowsPerThread + extraRow;

        pthread_mutex_init(&threadData[i].mutex, NULL); // Initialise a mutex for each thread

        // Assigning variables to structs.
        threadData[i].image2D = image2D;
        threadData[i].newImage2D = newImage2D;
        threadData[i].startOfRow = startOfRow;
        threadData[i].endOfRow = endOfRow;
        threadData[i].width = width;
        threadData[i].height = height;

        // Creating multiple threads
        pthread_create(&threadIDs[i], NULL, imageProcessing, (void*)&threadData[i]);
        startOfRow = endOfRow; //next thread will start processing from the row where the current thread left off.
    }

    // Joining threads ensures all tasks complete before further processing, preventing premature termination.
    for (int i = 0; i < threadCounts; i++) {
        pthread_join(threadIDs[i], NULL);
    }

    // Allocating memory for a 1D array for the new image
    unsigned char* image1DNew = malloc(width * height * 4 * sizeof(unsigned char));

    // Flatten 2D to 1D array for encoding
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width * 4; col++) {
            image1DNew[row * width * 4 + col] = newImage2D[row][col];
        }
    }

    // Free up allocated memory.
    free(image2D);
    free(newImage2D);

    //Iterate over each thread's data to destroy its associated mutex using a for loop.
    for (int i = 0; i < threadCounts; i++) {
        pthread_mutex_destroy(&threadData[i].mutex);
    }

    lodepng_encode32_file("blurredImage.png", image1DNew, width, height); // Encode to new image
    free(image1DNew); // Free resources

    printf("Success: Your image has been blurred and saved as 'blurredImage.png' in the current directory.");

}