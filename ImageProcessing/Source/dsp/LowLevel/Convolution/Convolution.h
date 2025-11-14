#ifndef IMAGEPROCESSING_CONVOLUTION_H
#define IMAGEPROCESSING_CONVOLUTION_H

#include "Core.hpp"
#include "Masks/Masks.h"

extern "C" {
	

	bool convolve(
		struct MaskAttributes* attrs, bool flipMask,
		uint32_t ImgRows, uint32_t ImgCols, uint32_t ImgSride, uint8_t ImgChannels, 
		void* input_buf , void* output_buf );

	bool image_convolve(
		struct MaskAttributes* attrs, bool flipMask ,
		struct bmpImage* input, struct bmpImage* output);
	
}

#endif 
