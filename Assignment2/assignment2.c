#include "libpnm.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv){
	int maxGray = 255;
	long int * frequencyArray = malloc((maxGray + 1) * sizeof(long int));
	
	//Test
	frequencyArray[0] = 19843;
	printf("my long int = %ld\n", frequencyArray[0]);
	return 0;
}

/**
* -Dynamically allocates long int array of (max_gray_value + 1) elements
* -Reads all pixels in the PGM image and records the frequency of occurence
*  of each pixel values in the allocated array
* -Store number_of_non_zero_values_in_frequency_array
* 
*INPUT
* input_pgm_image -> pointer to PGM_Image
* non_zero_frequency -> number of non zero values in the frequency array
*
*OUTPUT
* pointer to allocated array of non zero values
*/
long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, int *non_zero_frequency){
	int maxGray = input_pgm_image->maxGrayValue;
	long int * frequencyArray = malloc((maxGray + 1) * sizeof(long int));
	
	//Test
	frequencyArray[0] = 19843;
	printf("my long int = %ld\n", frequencyArray[0]);
	
	return frequencyArray;
}
