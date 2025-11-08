#ifndef IMAGEPROCESSING_CONVOLUTION_H
#define IMAGEPROCESSING_CONVOLUTION_H

#include "Core.hpp"

extern "C" {
	bool convolve(
		uint32_t MaskRows, uint32_t MaskCols, void* MaskData,
		uint32_t ImgRows, uint32_t ImgCols, uint32_t ImgSride, uint8_t ImgChannels, 
		void* input_buf , void* output_buf );
	bool image_convolve(
		uint32_t MaskRows, uint32_t MaskCols, void* MaskData,
		struct bmpImage* input, struct bmpImage* output);
	
}

#endif 
