#ifndef BMP_H
#define BMP_H


#define _CRT_SECURE_NO_WARNINGS
#include "../../Definitions.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif 
	extern uint8_t default_color_table[1024]; // 256 colors * 4 bytes per color (BGRA)

	PACKED_STRUCT(
		struct BMPHeader {
		uint16_t signature; // 0x4D42 'BM'
		uint32_t fileSize_unreleable; // size of the BMP file , but is not used
		uint16_t bfReserved1; // must be 0
		uint16_t bfReserved2; // must be 0
		uint32_t dataOffset; // offset to start of pixel data
		uint32_t size; // size of this header (40 bytes)
		int32_t width; // width of bitmap in pixels
		int32_t height; // height of bitmap in pixels
		uint16_t planes; // number of color planes, must be 1
		uint16_t bitCount; // number of bits per pixel
		uint32_t compression; // compression method being used
		uint32_t sizeImage; // size of the raw bitmap data
		int32_t xPelsPerMeter_unreleable; // horizontal resolution of the image , but is not used
		int32_t yPelsPerMeter_unreleable; // vertical resolution of the image , but is not used
		uint32_t clrUsed_unreleable; // number of colors in the color palette , but is not used
		uint32_t clrImportant_unreleable; // number of important colors used , but is not used

	}
	);

#define BMP_SIGNATURE 0x4D42

	struct bmpImage {
		struct BMPHeader header;
		// used metadata
		uint8_t* colorTable; // color table for indexed color images (can be NULL)
		uint8_t* pixels; // pixel data
		uint32_t width;
		uint32_t height;
		uint16_t channels; // number of channels (3 for RGB, 4 for RGBA)
	};

	bool bmp_load(const char* filename, struct bmpImage* image_out);
	
	bool bmp_store(const char* filename, const struct bmpImage* image_in);
#ifdef __cplusplus
}
#endif 
#endif 