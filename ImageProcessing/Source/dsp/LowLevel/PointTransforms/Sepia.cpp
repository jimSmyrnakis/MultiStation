#include "Sepia.h"
#include <math.h>
#include <memory.h>
extern "C" bool image_sepia(struct bmpImage* outputImage, const struct bmpImage* inputImage) {
	if (!inputImage || !outputImage || !inputImage->pixels) {
		return false;
	}

	// check if is rgb image
	if (inputImage->channels < 3) {
		return false; // Sepia filter requires at least 3 channels (RGB)
	}
	// Copy header information
	outputImage->header = inputImage->header;
	outputImage->width = inputImage->width;
	outputImage->height = inputImage->height;
	outputImage->channels = inputImage->channels;
	uint32_t bytesPerPixel = inputImage->channels;
	uint32_t rowSize = (inputImage->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * inputImage->height;
	// Allocate memory for output image pixels
	outputImage->pixels = (uint8_t*)malloc(totalBytes);
	if (!outputImage->pixels) {
		return false;
	}
	// Apply sepia filter
	for (uint32_t i = 0; i < inputImage->height; i++) {
		for (uint32_t j = 0; j < inputImage->width; j++) {
			uint32_t index = i * rowSize + j * bytesPerPixel;
			uint8_t r = inputImage->pixels[index + 2];
			uint8_t g = inputImage->pixels[index + 1];
			uint8_t b = inputImage->pixels[index + 0];
			// Calculate sepia values
			uint8_t tr = static_cast<uint8_t>(fmin(255.0f, 0.393f * r + 0.769f * g + 0.189f * b));
			uint8_t tg = static_cast<uint8_t>(fmin(255.0f, 0.349f * r + 0.686f * g + 0.168f * b));
			uint8_t tb = static_cast<uint8_t>(fmin(255.0f, 0.272f * r + 0.534f * g + 0.131f * b));
			outputImage->pixels[index + 2] = tr; // Red
			outputImage->pixels[index + 1] = tg; // Green
			outputImage->pixels[index + 0] = tb; // Blue
		}
	}
	// Copy color table if present
	outputImage->colorTable = nullptr;
	if (inputImage->colorTable != NULL) {
		outputImage->colorTable = (uint8_t*)malloc(inputImage->header.dataOffset - sizeof(struct BMPHeader));
		if (outputImage->colorTable == nullptr) {
			free(outputImage->pixels);
			return false; // Memory allocation failed
		}
		memcpy(outputImage->colorTable, inputImage->colorTable, inputImage->header.dataOffset - sizeof(struct BMPHeader));
	}

	return true;

}