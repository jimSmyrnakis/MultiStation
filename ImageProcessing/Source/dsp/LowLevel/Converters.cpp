#include "Converters.h"

extern "C" bool convert_rgb_to_grayscale_avarage(struct bmpImage* dest, const struct bmpImage* source) {
	if (!dest || !source || !source->pixels) {
		return false;
	}

	if (source->channels < 3) {
		// Source image must have at least 3 channels (RGB)
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = 1; // Grayscale
	uint32_t bytesPerPixelSrc = source->channels;
	uint32_t rowSizeSrc = (source->width * bytesPerPixelSrc + 3) & ~3;
	uint32_t rowSizeDest = (dest->width * dest->channels + 3) & ~3;
	uint32_t totalBytesDest = rowSizeDest * dest->height;

	dest->pixels = (uint8_t*)malloc(totalBytesDest);
	if (!dest->pixels) {
		return false;
	}

	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			uint32_t indexSrc = i * rowSizeSrc + j * bytesPerPixelSrc;
			uint32_t indexDest = i * rowSizeDest + j * dest->channels;
			uint8_t r = source->pixels[indexSrc + 2];
			uint8_t g = source->pixels[indexSrc + 1];
			uint8_t b = source->pixels[indexSrc + 0];
			// Convert to grayscale using luminosity method
			uint8_t gray = static_cast<uint8_t>((r + g + b )/3);
			dest->pixels[indexDest] = gray;
		}
	}

	dest->colorTable = default_color_table;
	dest->header.bitCount = 8;
	dest->header.dataOffset = sizeof(struct BMPHeader) + 1024;

	return true;
}

extern "C" bool convert_rgb_to_grayscale_luminosity(struct bmpImage* dest, const struct bmpImage* source , float r_s, float g_s, float b_s) {
	if (!dest || !source || !source->pixels) {
		return false;
	}

	if (source->channels < 3) {
		// Source image must have at least 3 channels (RGB)
		return false;
	}

	dest->header = source->header;
	dest->width = source->width;
	dest->height = source->height;
	dest->channels = 1; // Grayscale
	uint32_t bytesPerPixelSrc = source->channels;
	uint32_t rowSizeSrc = (source->width * bytesPerPixelSrc + 3) & ~3;
	uint32_t rowSizeDest = (dest->width * dest->channels + 3) & ~3;
	uint32_t totalBytesDest = rowSizeDest * dest->height;

	dest->pixels = (uint8_t*)malloc(totalBytesDest);
	if (!dest->pixels) {
		return false;
	}

	for (uint32_t i = 0; i < dest->height; i++) {
		for (uint32_t j = 0; j < dest->width; j++) {
			uint32_t indexSrc = i * rowSizeSrc + j * bytesPerPixelSrc;
			uint32_t indexDest = i * rowSizeDest + j * dest->channels;
			uint8_t r = source->pixels[indexSrc + 2];
			uint8_t g = source->pixels[indexSrc + 1];
			uint8_t b = source->pixels[indexSrc + 0];
			// Convert to grayscale using luminosity method
			uint16_t gray = static_cast<uint16_t>(r_s * r + g_s * g + b_s * b);
			if (gray > 255)
				gray = 255;
			dest->pixels[indexDest] = gray;
		}
	}

	dest->colorTable = default_color_table;
	dest->header.bitCount = 8;
	dest->header.dataOffset = sizeof(struct BMPHeader) + 1024;

	return true;
}