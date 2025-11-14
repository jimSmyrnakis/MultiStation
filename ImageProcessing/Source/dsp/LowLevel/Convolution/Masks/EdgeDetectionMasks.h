#ifndef IMAGEPROCESSING_EDGE_DETECTION_H
#define IMAGEPROCESSING_EDGE_DETECTION_H

#include "Core.hpp"
#include "Masks.h"
extern "C" {
	
	

	bool prewwitMask(enum MaskType type , struct MaskAttributes* attrs);
	bool sobelMask(enum MaskType type, struct MaskAttributes* attrs);
	bool robinsonMask(enum MaskType type, struct MaskAttributes* attrs);
	bool KirschMask(enum MaskType type, struct MaskAttributes* attrs);
	bool laplaceMask(enum MaskType type, struct MaskAttributes* attrs);
	bool robertsMask(enum MaskType type, struct MaskAttributes* attrs);
}

#endif 
