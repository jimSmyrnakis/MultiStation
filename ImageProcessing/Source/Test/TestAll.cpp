#include "inc.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>

int TestAll(void) {
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
		return 0;
		return 1;
	}

	struct bmpImage InvImage;
	if (image_inverse(&InvImage, &image) == false)
	{
		printf("Failed to invert image.\n");
		return 1;
	};

	struct bmpImage NoisyImage;
	if (image_salt_papper_noise(&NoisyImage, &image, 0.6f) == false) {
		printf("Failed to Add noise on the image.\n");
		return 1;
	}

	struct bmpImage grayImage;
	if (convert_rgb_to_grayscale_luminosity(&grayImage, &image, .1f, .1f, .8f) == false)
	{

		printf("Failed to convert image to grayscale.\n");
		grayImage = image;
		//return 1;
	};


	uint8_t threshold = 127;
	struct bmpImage binaryImage;
	if (image_binarize(&binaryImage, &grayImage, threshold , {malloc , free}) == false)
	{
		printf("Failed to binarize image.\n");
		return 1;
	}

	struct bmpImage brightGrayScaleImage;
	if (add_images_truncate(&brightGrayScaleImage, 80, &grayImage) == false)
	{
		printf("Failed to add images.\n");
		return 1;
	}

	struct bmpImage brightOriginalImage;
	if (add_images_truncate(&brightOriginalImage, 30, &image) == false)
	{
		printf("Failed to add images.\n");
		return 1;
	}

	struct bmpImage darkerOriginalImage;
	if (subtract_images_truncate(&darkerOriginalImage, 30, &image) == false)
	{
		printf("Failed to add images.\n");
		return 1;
	}

	struct bmpImage gammaImage;
	if (image_gamma_transform(&gammaImage, &image, 1.1, 0, 1) == false)
	{
		printf("Failed to add images.\n");
		return 1;
	}

	struct bmpImage equalizedImage;
	if (!image_histogram_equalization(&equalizedImage, &brightGrayScaleImage)) {
		printf("Failed to perform histogram equalization.\n");
		return 1;
	}

	struct bmpImage equalizedImage2;
	if (!image_histogram_equalization(&equalizedImage2, &equalizedImage)) {
		printf("Failed to perform histogram equalization.\n");
		return 1;
	}

	struct bmpImage rotatedImage;
	if (!Image_rotate(&equalizedImage, &rotatedImage, 3.141592f/2)) {
		printf("Failed to rotate image.\n");
		return 1;
	}

	struct bmpImage GausianNoiseImage;
	if (!image_GausianNoise(&GausianNoiseImage, &image, 90, 80, { malloc, free })) {
		printf("Failed to make Gauian Noise image.\n");
		return 1;
	}

	

	

	struct bmpImage lineDetectorImage;
	struct MaskAttributes* attrs = MaskGet(FILTER_LOW_PASS_SPATIAL);
	if (!image_convolve( attrs, true , &image, &lineDetectorImage )) {
		printf("Failed to apply convolve filter.\n");
		return 1;
	}

	


	// store a copy image to another file
	if (bmp_store(".//Application//Assets//grayscale.bmp", &grayImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//NoisyImage.bmp", &NoisyImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//GrayScaleBinarized.bmp", &binaryImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//GrayScaleBright.bmp", &brightGrayScaleImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//OriginalBright.bmp", &brightOriginalImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//OriginalDarker.bmp", &darkerOriginalImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//gamma.bmp", &gammaImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//HistogramEquallized.bmp", &equalizedImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//rotatedImage.bmp", &rotatedImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//HistogramEquallized2.bmp", &equalizedImage2)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//InvertedImage.bmp", &InvImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	if (bmp_store(".//Application//Assets//Gausian.bmp", &GausianNoiseImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	

	if (bmp_store(".//Application//Assets//LineDetectImage.bmp", &lineDetectorImage)) {
		printf("Stored BMP image successfully.\n");
	}
	else {
		printf("Failed to store BMP image.\n");
	}

	// Histogram
	float* histogram = image_histogram_init(&brightGrayScaleImage, true);
	if (histogram == nullptr) {
		printf("Failed to compute histogram.\n");
		return 1;
	}
	if (!histogram_write_to_file(".//Application//Assets//GrayBrightImageHistogram.txt", histogram, 256)) {
		printf("Failed to write histogram to file.\n");
		free(histogram);
		return 1;
	}
	else {
		printf("Histogram written to file successfully.\n");
	}

	float* histogram_eq = image_histogram_init(&equalizedImage, true);
	if (histogram_eq == nullptr) {
		printf("Failed to compute histogram.\n");
		return 1;
	}
	if (!histogram_write_to_file(".//Application//Assets//GrayBrightImageHistogramEquallized.txt", histogram_eq, 256)) {
		printf("Failed to write histogram to file.\n");
		free(histogram_eq);
		return 1;
	}
	else {
		printf("Histogram written to file successfully.\n");
	}


	free(histogram_eq);
	free(brightOriginalImage.pixels);
	free(darkerOriginalImage.pixels);
	free(gammaImage.pixels);
	free(equalizedImage.pixels);
	free(histogram);
	//free(grayImage.pixels);
	free(binaryImage.pixels);
	free(brightGrayScaleImage.pixels);
	free(image.pixels);

	return 0;
}