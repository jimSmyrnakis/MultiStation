#ifndef BINARIZATION_H
#define BINARIZATION_H

	#include "Core.hpp"

	#ifdef __cplusplus
		extern "C" {
	#endif 
			bool image_binarize(
				struct bmpImage* dest,
				const struct bmpImage* source,
				uint8_t threshold ,
				struct Allocator allocator
				);

			bool binarize_grayscale(
				uint32_t height, uint32_t stride, uint32_t width,
				void* in_buf, void* out_buf , uint8_t threshold
			);

	#ifdef __cplusplus
			
		}
	#endif 

#endif 