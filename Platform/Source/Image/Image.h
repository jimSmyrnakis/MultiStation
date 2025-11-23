#ifndef PLATFORM_SOURCE_IMAGE_IMAGE_H
#define PLATFORM_SOURCE_IMAGE_IMAGE_H
	
#include "../Definitions.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	/** Version 1.0 of Image Generic Format */
	struct Image {
		uint32_t iWidth; // image width in pixels
		uint32_t iHeight; // image height in pixels
		uint32_t iStride; // number of bytes per row (including padding)
		uint8_t  channels; // number of channels (e.g., 3 for RGB, 4 for RGBA) max of 4 channels
		uint8_t  bitsPerChannel[4]; // bits per channel for up to 4 channels support
		uint8_t  planes; // Number of planes the pixels are divide to 
		void*    pData;   // pointer to pixel data
		uint16_t id;	// Identify the type of format this image haved . 
		char     type[64]; // Like jpeg , png , bmp etc. 
	};

	//typedef int (*ImageResolve)()
	//int ImageRead(const char* filename, struct Image* image , struct Allocator allocator );
	//int ImageCreate(struct ImageAttributes attributes, struct Image* image, struct Allocator allocator);
	//int ImageWrite(const char* filename, struct Image* image);


#ifdef __cplusplus
}
#endif


#endif 
