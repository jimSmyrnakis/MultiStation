#include "GammaTransform.h"
#include <memory.h>
#include <math.h>
extern "C" bool image_gamma_transform(
	struct bmpImage* dest, const struct bmpImage* source,
	float gamma, float epsilon, float c) {

	if (!source || !dest || gamma < 0 || c < 0 || epsilon < 0) {
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = source->channels;
	dest->colorTable = NULL;
	if (source->colorTable) {
		dest->colorTable = (uint8_t*)malloc(source->header.dataOffset - sizeof(struct BMPHeader));
		if (!dest->colorTable) {
			return false;
		}
		memcpy_s(
			dest->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader),
			source->colorTable,
			source->header.dataOffset - sizeof(struct BMPHeader)
		);
	}
	

	uint32_t bytesPerPixel = source->channels;
	uint32_t rowSize = (source->width * bytesPerPixel + 3) & ~3;
	uint32_t totalBytes = rowSize * dest->height;
	dest->pixels = (uint8_t*)malloc(totalBytes);
	if (!dest->pixels) {
		return false;
	}


	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			for (uint32_t k = 0; k < dest->channels; k++) {
				uint32_t index = i * rowSize + j * bytesPerPixel + k;
				float gamma_correction;
				gamma_correction = c *pow( (source->pixels[index] + epsilon), gamma);
				if (gamma_correction > 255.0f) gamma_correction = 255.0f;
				dest->pixels[index] = (uint8_t)(gamma_correction);
			}
		}
	}

	
	

	return true;
}