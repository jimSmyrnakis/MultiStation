#include "Blur.h"

bool image_blur(
	struct bmpImage* outputImage, 
	const struct bmpImage* inputImage, 
	int kernelSize,
	struct Allocator allocator ) {
	if (inputImage == NULL || outputImage == NULL || kernelSize <= 0 || !allocator.memalloc || !allocator.memfree) {
		return false; // Invalid parameters
	}
	int width = inputImage->width;
	int height = inputImage->height;
	int channels = inputImage->channels;
	// Initialize output image
	outputImage->width = width;
	outputImage->height = height;
	outputImage->channels = channels;
	outputImage->header = inputImage->header;
	outputImage->pixels = (uint8_t*)allocator.memalloc(width * height * channels);
	if (outputImage->pixels == NULL) {
		return false; // Memory allocation failed
	}

	// color table
	outputImage->colorTable = NULL;
	if (inputImage->colorTable != NULL) {
		outputImage->colorTable = (uint8_t*)allocator.memalloc(inputImage->header.dataOffset - sizeof(struct BMPHeader));
		if (outputImage->colorTable == NULL) {
			free(outputImage->pixels);
			return false; // Memory allocation failed
		}
		memcpy(outputImage->colorTable, inputImage->colorTable, inputImage->header.dataOffset - sizeof(struct BMPHeader));
	}

	// Create a simple box blur kernel
	int kernelDim = kernelSize;
	int kernelArea = kernelDim * kernelDim;
	// Allocate the kernel 
	float* kernel = (float*)allocator.memalloc(kernelArea * sizeof(float));
	if (kernel == NULL) {
		allocator.memfree(outputImage->pixels);
		allocator.memfree(outputImage->colorTable);
		return false; // Memory allocation failed
	}

	// Fill the kernel with equal weights
	for (int i = 0; i < kernelArea; i++) {
		kernel[i] = 1.0f / kernelArea;
		if (i == (kernelDim / 2)) {
			kernel[i] = 1.0f; // the current filter pixel will have more weight
		}
	}

	// Apply the blur filter
	// Loop through each pixel in the input image
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < channels; c++) {
				float bluredValue = 0.0f;
				for (int i = 0; i < kernelArea; i++) {
					int kernelX = i % kernelDim;
					int kernelY = i / kernelDim;
					int imageX = x + kernelX - kernelDim / 2;
					int imageY = y + kernelY - kernelDim / 2;
					// Wrap around the image edges
					if (imageX < 0) {
						//imageX = width + imageX;
						imageX = 0;
					}
					else if (imageX >= width) {
						imageX = imageX - width;
						imageX = width - 1;
					}

					if (imageY < 0) {
						imageY = height + imageY;
						imageY = 0;
					}
					else if (imageY >= height) {
						imageY = imageY - height;
						imageY = height - 1;
					}

					uint8_t pixelValue = inputImage->pixels[(imageY * width + imageX) * channels + c];
					bluredValue += pixelValue * kernel[i];
					
				}

				if (bluredValue <= 0.0f) bluredValue = 1.0f;
				if (bluredValue > 255.0f) bluredValue = 255.0f;
				
				// copy the blurred value to the output image
				outputImage->pixels[(y * width + x) * channels + c] = (uint8_t)(bluredValue);
			}
		}
	}

	
	return true;
}

