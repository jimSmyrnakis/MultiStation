#include "Core.hpp"
#include "Test/inc.h"
#include "dsp/LowLevel/PointTransforms/GausianNoise.h"
#include "dsp/LowLevel/Filters/Filters.h"
#include "dsp/LowLevel/Convolution/Convolution.h"

#define FOLDER(name) "./Application/Assets/Ergasia1Images/Askisi3/"##name
#define CHECK(func) if (!(func)){ ASSERT(0 , "Failed !!!"); return 1;}
int Erg1Ask3(void) {


	// load the image
	bmpImage NotGrey;
	bmpImage OriginalImage;
	CHECK(bmp_load(FOLDER("clock.bmp"), &NotGrey));
	CHECK(convert_rgb_to_grayscale_avarage(&OriginalImage, &NotGrey));


	// SOBEL 
	MaskAttributes* SobelxMask = MaskGet(MaskType::EDGE_DETECTION_SOBEL_VERTICAL);
	MaskAttributes* SobelyMask = MaskGet(MaskType::EDGE_DETECTION_SOBEL_HORIZONTAL);
	
	bmpImage SobelXImage;
	bmpImage SobelYImage;
	bmpImage SobelImage;

	CHECK(image_convolve(SobelxMask, false, &OriginalImage, &SobelXImage));
	CHECK(image_convolve(SobelyMask, false, &OriginalImage, &SobelYImage));
	CHECK(ImageMagnitute(&SobelImage, &SobelXImage, &SobelYImage));

	bmpImage BinarizedSobelImage;
	CHECK(image_binarize(&BinarizedSobelImage, &SobelImage, 48, { malloc , free }));


	CHECK(bmp_store(FOLDER("sobel.bmp"), &SobelImage));
	CHECK(bmp_store(FOLDER("binarized_sobel.bmp"), &BinarizedSobelImage));

	// PREWWIT
	MaskAttributes* PrewwitxMask = MaskGet(MaskType::EDGE_DETECTION_PREWITT_VERTICAL);
	MaskAttributes* PrewwityMask = MaskGet(MaskType::EDGE_DETECTION_PREWITT_HORIZONTAL);

	bmpImage PrewwitXImage;
	bmpImage PrewwitYImage;
	bmpImage PrewwitImage;

	CHECK(image_convolve(PrewwitxMask, false, &OriginalImage, &PrewwitXImage));
	CHECK(image_convolve(PrewwityMask, false, &OriginalImage, &PrewwitYImage));
	CHECK(ImageMagnitute(&PrewwitImage, &PrewwitXImage, &PrewwitYImage));

	bmpImage BinarizedPrewwitImage;
	CHECK(image_binarize(&BinarizedPrewwitImage, &PrewwitImage, 32 , {malloc , free}));

	CHECK(bmp_store(FOLDER("prewwit.bmp"), &PrewwitImage));
	CHECK(bmp_store(FOLDER("binarized_prewwit.bmp"), &BinarizedPrewwitImage));


	// Gausian Filter
	bmpImage GausianFilteredImage;
	float sigma = 1.0f;
	uint32_t kernelSize = 5;
	CHECK(ImageGausianFilter(&GausianFilteredImage, &OriginalImage, kernelSize, sigma));
	CHECK(bmp_store(FOLDER("gausian_filtered.bmp"), &GausianFilteredImage));

	// Canny Edge Detector
	bmpImage CannyEdgeImage;
	sigma = 1;
	CHECK(imageCannyEdgeDetect(&CannyEdgeImage, &OriginalImage, sigma, 16, 32));
	CHECK(bmp_store(FOLDER("canny_edge2.bmp"), &CannyEdgeImage));

	return 0;
}
