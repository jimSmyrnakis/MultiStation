#include "Binarization.h"

bool image_binarize(
	struct bmpImage* dest,
	const struct bmpImage* source,
	uint8_t threshold,
	struct Allocator allocator ) {

	if (!dest || !source || !source->pixels || !allocator.memalloc) {
		return false;
	}

	if (source->channels != 1) {
		// Source image must have at least 1 channel (Grayscale)
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = 1; // Binarized image

	uint32_t bytesPerPixelSrc = source->channels;
	uint32_t rowSizeSrc = (source->width * bytesPerPixelSrc + 3) & ~3;
	uint32_t rowSizeDest = (dest->width * dest->channels + 3) & ~3;
	uint32_t totalBytesDest = rowSizeDest * dest->height;
	dest->pixels = (uint8_t*)allocator.memalloc(totalBytesDest);
	if (!dest->pixels) {
		return false;
	}
	dest->colorTable = default_color_table;


	return binarize_grayscale(
		source->height, rowSizeDest, 
		source->width, source->pixels, 
		dest->pixels, threshold);
}

bool binarize_grayscale(
	uint32_t height, uint32_t stride, uint32_t width, 
	void* in_buf, void* out_buf, uint8_t threshold
) {
	if (!height || !width || stride < width || !in_buf || !out_buf) {
		return false;
	}

	uint8_t* iPixels = (uint8_t*)in_buf;
	uint8_t* oPixels = (uint8_t*)out_buf;

	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			uint32_t index = y * stride + x;
			uint8_t iVal = iPixels[index];
			oPixels[y * stride + x] = (iVal < threshold) ? 0 : 255;
		}
	}

	return true;
}