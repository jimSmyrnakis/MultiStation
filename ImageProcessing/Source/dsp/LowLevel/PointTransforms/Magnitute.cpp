#include "Magnitute.h"


extern "C" bool ImageMagnitute(struct bmpImage* dest, const bmpImage* src1, const bmpImage* src2) {
	if (!dest || !src1 || !src2) {
		return false;

	}

	if (src1->channels != src2->channels) return false;

	if ((src1->width != src2->width) && (src1->height != src2->height)) return false;



	dest->header = src1->header;
	dest->width = src1->width;
	dest->height = src1->height;
	dest->channels = src1->channels;
	uint32_t bytesPerPixel = src1->channels;
	uint32_t rowSize = (src1->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * dest->height;
	dest->pixels = (uint8_t*)malloc(totalBytes);
	if (!dest->pixels) {
		return false;
	}
	// color table
	if (src1->colorTable) {
		dest->colorTable = (uint8_t*)malloc(src1->header.dataOffset - sizeof(struct BMPHeader));
		if (
			!dest->colorTable) {
			return false;
		}
		memcpy_s(
			dest->colorTable,
			src1->header.dataOffset - sizeof(struct BMPHeader),
			src1->colorTable,
			src1->header.dataOffset - sizeof(struct BMPHeader)
		);
	}
	else {
		dest->colorTable = NULL;
	}

	// magnitute now
	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			for (uint32_t c = 0; c < dest->channels; c++) {
				uint32_t index = i * rowSize + j * bytesPerPixel + c;
				float val = sqrt(pow(src1->pixels[index], 2) + pow(src2->pixels[index], 2));
				if (val > 255) val = 255;
				if (val < 0) val = 0;

				dest->pixels[index] = (uint8_t)val;
			}
		}
	}


	return true;
}