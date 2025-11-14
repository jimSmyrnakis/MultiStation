#include "Operations.h"
#include <cmath>
#include <memory>
static uint64_t max_pixels_value(const struct bmpImage* img) {
	uint64_t max_value = 0;
	if (!img || !img->header.bitCount) {
		return 0;
	}

	uint64_t depth = img->header.bitCount;


	max_value = (depth < 8) ? ((1 << depth) - 1) : 255;
	
	return max_value;
}



extern "C" bool add_images_truncate(
	struct bmpImage* dest,
	uint8_t value,
	const struct bmpImage* source) {

	if (!dest || !source || !source->pixels) {
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	dest->colorTable =  (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
	if (
		!dest->colorTable) {
		return false;
	}
	memcpy_s(
		dest->colorTable,
		source->header.dataOffset - sizeof(struct BMPHeader),
		source->colorTable,
		source->header.dataOffset - sizeof(struct BMPHeader)
	);

	uint32_t bytesPerPixel = source->channels;
	uint32_t rowSize = (source->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * dest->height;
	dest->pixels = (uint8_t*)malloc(totalBytes);
	if (!dest->pixels) {
		return false;
	}

	uint64_t max_val = max_pixels_value(source);

	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			for (uint32_t c = 0; c < dest->channels; c++) {
				uint32_t index = i * rowSize + j * bytesPerPixel + c;
				uint64_t sum = (uint64_t)(source->pixels[index]) + (uint64_t)(value);
				if (sum > max_val) {
					sum = max_val; // Truncate to 255
				}
				dest->pixels[index] = static_cast<uint8_t>(sum);
			}
		}
	}



	return true;

}

extern "C" bool subtract_images_truncate(
	struct bmpImage* dest,
	uint8_t value,
	const struct bmpImage* source) {

	if (!dest || !source || !source->pixels) {
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	dest->colorTable = (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
	if (
		!dest->colorTable) {
		return false;
	}
	memcpy_s(
		dest->colorTable,
		source->header.dataOffset - sizeof(struct BMPHeader),
		source->colorTable,
		source->header.dataOffset - sizeof(struct BMPHeader)
	);

	uint32_t bytesPerPixel = source->channels;
	uint32_t rowSize = (source->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * dest->height;
	dest->pixels = (uint8_t*)malloc(totalBytes);
	if (!dest->pixels) {
		return false;
	}

	uint64_t max_val = max_pixels_value(source);

	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			for (uint32_t c = 0; c < dest->channels; c++) {
				uint32_t index = i * rowSize + j * bytesPerPixel + c;
				int64_t sum = (int64_t)(source->pixels[index]) - (int64_t)(value);
				if (sum < 0) {
					sum = 0; // Truncate to 255
				}
				dest->pixels[index] = static_cast<uint8_t>(sum);
			}
		}
	}



	return true;

}