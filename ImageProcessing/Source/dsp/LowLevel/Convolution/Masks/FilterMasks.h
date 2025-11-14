#ifndef IMAGEPROCESSING_FILTER_MASKS_H
#define IMAGEPROCESSING_FILTER_MASKS_H

#include "Core.hpp"
#include "Masks.h"
extern "C" {



	bool highPassSpatialMask(enum MaskType type, struct MaskAttributes* attrs);
	bool lowPassSpatialMask(enum MaskType type, struct MaskAttributes* attrs);
	
}

#endif 

