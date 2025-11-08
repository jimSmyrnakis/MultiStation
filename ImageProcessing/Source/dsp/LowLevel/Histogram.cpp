#include "Histogram.h"
#include <memory.h>
#define GRAYSCALE_HISTOGRAM_SIZE 256
#define GRAYSCALE_L_MAX 255
extern "C" float* image_histogram_init(const struct bmpImage* image , bool normallize) {

	if (image == nullptr || image->pixels == nullptr) {
		return nullptr; // Invalid image
	}

	//check if image is grayscale
	if (image->channels != 1) {
		return nullptr; // Not a grayscale image
	}

	// Allocate memory for histogram (256 bins for grayscale)
	float* histogram = (float*)malloc(GRAYSCALE_HISTOGRAM_SIZE * sizeof(float) );
	if (histogram == nullptr) {
		return nullptr; // Memory allocation failed
	}

	// Initialize histogram bins to zero
	for (size_t i = 0; i < GRAYSCALE_HISTOGRAM_SIZE; ++i) {
		histogram[i] = 0.0f;
	}

	// Calculate histogram
	size_t totalPixels = image->width * image->height;
	for (size_t i = 0; i < totalPixels; ++i) {
		uint8_t pixelValue = image->pixels[i]; // Assuming grayscale image
		histogram[pixelValue]++;
	}

	// Normalize histogram if required
	for (size_t i = 0; (i < GRAYSCALE_HISTOGRAM_SIZE) && normallize; ++i) {
		histogram[i] = (histogram[i] ) / totalPixels; // Scale to 0-255
	}

	
	return histogram;
}


extern "C" bool histogram_write_to_file(const char* filename, float* histogram, size_t size) {
	if (!filename || !histogram || size == 0) {
		return false;
	}

	FILE* file = fopen(filename, "w");
	if (!file) {
		return false;
	}

	for (size_t i = 0; i < size; ++i) {
		fprintf(file, "%zu %f\n", i, histogram[i]);
	}

	fclose(file);

	return true;
}

extern "C" bool image_histogram_equalization(struct bmpImage* dest, const struct bmpImage* source) {
	if (!dest || !source || !source->pixels) {
		return false;
	}

	float* histogram = image_histogram_init(source, true);
	if (!histogram) {
		return false;
	}

	// Compute cumulative distribution function (CDF)
	float cdf[GRAYSCALE_HISTOGRAM_SIZE] = { 0 };
	cdf[0] = histogram[0];
	for (size_t i = 1; i < GRAYSCALE_HISTOGRAM_SIZE; ++i) {
		cdf[i] = cdf[i - 1] + histogram[i];
	}

	// Map old pixel values to new pixel values using CDF
	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	uint32_t rowSize = (source->width * source->channels + 3) & ~3;
	dest->pixels = (uint8_t*)malloc(rowSize * dest->height);
	if (!dest->pixels) {
		free(histogram);
		return false;
	}

	for (uint32_t i = 0; i < dest->height; ++i) {
		for (uint32_t j = 0; j < dest->width; ++j) {
			uint32_t index = i * rowSize + j * dest->channels;
			uint8_t oldPixelValue = source->pixels[index];
			uint8_t newPixelValue = 
			static_cast<uint8_t>(((cdf[oldPixelValue] - cdf[0]) / (1.0f - cdf[0])) * GRAYSCALE_L_MAX + 0.5f);
			dest->pixels[index] = newPixelValue;
		}
	}

	free(histogram);




	// copy color table if exists
	if (source->colorTable) {
		dest->colorTable = (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
		if (!dest->colorTable) {
			free(dest->pixels);
			return false;
		}
		memcpy_s(
			dest->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader),
			source->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader)
		);
	}
	else {
		dest->colorTable = nullptr;
	}

	return true;
}