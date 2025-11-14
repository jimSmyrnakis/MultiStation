#include "inc.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>

int TestLineDetector(void) {
	struct bmpImage image;
	if (bmp_load(".//Application//Assets//goldhill.bmp", &image)) {
		printf("Loaded BMP image: %dx%d, Channels: %d\n", image.width, image.height, image.channels);
		// Successfully loaded the BMP image
		// Process the image as needed
		// Free the allocated pixel data after processing

	}
	else {
		// Failed to load the BMP image
		printf("Loaded BMP image: failed :(\n");
		// print procees directory
		printf("Current working directory: ");

		char buffer[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, buffer);
		std::cout << "Current directory: " << buffer << std::endl;
		return 1;
	}


	struct bmpImage grayImage;
	if (convert_rgb_to_grayscale_luminosity(&grayImage, &image, .1f, .1f, .8f) == false)
	{

		printf("Failed to convert image to grayscale.\n");
		return 1;
	};


	
	// store a copy image to another file
	if (bmp_store(".//Application//Assets//grayscale.bmp", &grayImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}



	





	
	free(grayImage.pixels);
	free(image.pixels);
	return 0;
}
