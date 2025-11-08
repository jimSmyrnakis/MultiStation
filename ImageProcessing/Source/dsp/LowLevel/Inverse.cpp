#include "Inverse.h"
#include <memory.h>
extern "C" bool image_inverse(
	struct bmpImage* dest,
	const struct bmpImage* source) {
	if (!dest || !source || !source->pixels) {
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	uint32_t bytesPerPixel = source->channels;
	uint32_t rowSize = (source->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * dest->height;
	dest->pixels = (uint8_t*)malloc(totalBytes);
	if (!dest->pixels) {
		return false;
	}
	// color table
	if (source->colorTable) {
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
	}
	else {
		dest->colorTable = NULL;
	}

	// inverse now
	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			for (uint32_t c = 0; c < dest->channels; c++) {
				uint32_t index = i * rowSize + j * bytesPerPixel + c;
				dest->pixels[index] = 255 - source->pixels[index];
			}
		}
	}

	return true;
}