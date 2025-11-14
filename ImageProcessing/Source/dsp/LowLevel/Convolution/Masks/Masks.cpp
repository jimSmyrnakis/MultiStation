#include "Masks.h"
#include <memory.h>
#include "LineDetectionMask.h"
#include "EdgeDetectionMasks.h"
#include "Other.h"
#include "FilterMasks.h"

extern "C" {
	
	MaskResolveFunc Maps[] = {
		LineDetectionMask , 
		prewwitMask , 
		sobelMask , 
		robinsonMask , 
		KirschMask , 
		laplaceMask ,
		robertsMask ,
		lowPassSpatialMask ,
		highPassSpatialMask
	};

	struct MaskAttributes* MaskGet(enum MaskType type) {
		
		struct MaskAttributes* mask = (struct MaskAttributes*)malloc(sizeof(struct MaskAttributes));
		ASSERT(mask, "Out of memory !!!");

		for (int i = 0; i < sizeof(Maps) / sizeof(MaskResolveFunc); i++) {
			if (Maps[i](type, mask)) {
				return mask;
			}
		}

		free(mask);
		ASSERT(0, "Mask either not defined or not implemented :( !!!");
		return NULL;
	}

}