#include "Core.hpp"
#include "Test/inc.h"
#include "dsp/LowLevel/PointTransforms/GausianNoise.h"
#include "dsp/LowLevel/Filters/Filters.h"
#include "dsp/LowLevel/Convolution/Convolution.h"

#define FOLDER(name) "./Application/Assets/Ergasia1Images/Askisi1/"##name
#define CHECK(func) if (!(func)){ ASSERT(0 , "Failed !!!"); return 1;}
int Erg1Ask1(void) {
	

	// load the image
	bmpImage OriginalImage;
	CHECK(bmp_load(FOLDER("pepper.bmp"), &OriginalImage));








	// Salt And Papper Noise 20%
	bmpImage SaltAndPapperNoise20;
	CHECK(image_salt_papper_noise(&SaltAndPapperNoise20, &OriginalImage, .2));
	CHECK(bmp_store(FOLDER("SaltPepper20perc.bmp"), &SaltAndPapperNoise20));

	// Gausian Noise 15db
	bmpImage GausianNoise15db;
	CHECK(image_GausianNoise(&GausianNoise15db, &OriginalImage, 4));
	CHECK(bmp_store(FOLDER("GausianNoise15db.bmp"), &GausianNoise15db));

	// Compine Salt & Pepper and Gausian Noise
	bmpImage CompinationalNoise;
	CHECK(image_GausianNoise(&CompinationalNoise, &SaltAndPapperNoise20, 4)) ;
	CHECK(bmp_store(FOLDER("CompinationalNoise.bmp"), &CompinationalNoise));

	//Avarage Fitler mask
	MaskAttributes* avgMask = MaskGet(MaskType::FILTER_LOW_PASS_SPATIAL); // Avg Mask 
	// Filter's of Gausian Noise
	bmpImage MedianFilterGausianNoise;
	bmpImage AvarageFilterGausian;
	CHECK(ÉmageMedianFilter(&MedianFilterGausianNoise, &GausianNoise15db, 3));
	CHECK(image_convolve(avgMask, false, &GausianNoise15db, &AvarageFilterGausian));
	CHECK(bmp_store(FOLDER("GausianNoiseAvarageFilter.bmp"), &AvarageFilterGausian));
	CHECK(bmp_store(FOLDER("GausianNoiseMedianFilter.bmp"), &MedianFilterGausianNoise));


	//Filter's of Salt and Pepper Noise
	bmpImage MedianFilterSaltPepperNoise;
	bmpImage AvarageFilterSaltPepperNoise;
	CHECK(ÉmageMedianFilter(&MedianFilterSaltPepperNoise, &SaltAndPapperNoise20, 3));
	CHECK(image_convolve(avgMask, false, &SaltAndPapperNoise20, &AvarageFilterSaltPepperNoise));
	CHECK(bmp_store(FOLDER("SaltAndPepperAvarageFilter.bmp"), &AvarageFilterSaltPepperNoise));
	CHECK(bmp_store(FOLDER("SaltAndPepperMedianFilter.bmp"), &MedianFilterSaltPepperNoise));


	//Filter's of Compinational Noise
	bmpImage MedianFilterCompinationalNoise;
	bmpImage AvarageFilterCompinationalNoise;
	bmpImage CompinationalFilterCompinationalNoise;
	CHECK(ÉmageMedianFilter(&MedianFilterCompinationalNoise  ,&CompinationalNoise, 3));
	CHECK(image_convolve(avgMask, false, &CompinationalNoise, &AvarageFilterCompinationalNoise));
	CHECK(image_convolve(avgMask, false, &MedianFilterCompinationalNoise, &CompinationalFilterCompinationalNoise));
	CHECK(bmp_store(FOLDER("CompinationalAvarageFilter.bmp"), &AvarageFilterCompinationalNoise));
	CHECK(bmp_store(FOLDER("CompinationalMedianFilter.bmp"), &MedianFilterCompinationalNoise));
	CHECK(bmp_store(FOLDER("CompinationalNoiseCompinationalFilters.bmp"), &CompinationalFilterCompinationalNoise));








	return 0;
}
