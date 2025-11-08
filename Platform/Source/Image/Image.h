#ifndef PLATFORM_SOURCE_IMAGE_IMAGE_H
#define PLATFORM_SOURCE_IMAGE_IMAGE_H
	
#include "../definitions.hpp"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct ImageAttributes {
		
	};

	struct Image {
		uint32_t iWidth; // image width in pixels
		uint32_t iHeight; // image height in pixels
		uint32_t iStride; // number of bytes per row (including padding)
		uint8_t  channels; // number of channels (e.g., 3 for RGB, 4 for RGBA) max of 4 channels
		uint8_t  bitsPerChannel[4]; // bits per channel for up to 4 channels 
		void* pixelData;   // pointer to pixel data
	};

	//int ImageRead(const char* filename, struct Image* image , struct Allocator allocator );
	//int ImageCreate(struct ImageAttributes attributes, struct Image* image, struct Allocator allocator);
	//int ImageWrite(const char* filename, struct Image* image);


#ifdef __cplusplus
}
#endif


#endif 
