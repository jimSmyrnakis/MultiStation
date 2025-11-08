#ifndef BLUR_H
#define BLUR_H

#include "Core.hpp"
	#ifdef __cplusplus
		extern "C" {
	#endif 
			bool image_blur(
				struct bmpImage* outputImage, 
				const struct bmpImage* inputImage, 
				int kernelSize , 
				struct Allocator allocator);

	#ifdef __cplusplus
		}
	#endif 

#endif 
