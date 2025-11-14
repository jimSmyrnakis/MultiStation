#include "EdgeDetectionMasks.h"

extern "C" {

	

	static float vert[3][3] = {
		{-1 , 0 , 1},
		{-1 , 0 , 1},
		{-1 , 0 , 1}
	};

	static float hori[3][3] = {
		{-1 , -1 , -1},
		{ 0 ,  0 ,  0},
		{ 1 ,  1 ,  1}
	};

	bool prewwitMask(enum MaskType type, struct MaskAttributes* attrs) {
		ASSERT(attrs, "Bad Parameter !!!");

		switch (type) {
		case EDGE_DETECTION_PREWITT_VERTICAL: attrs->mask = vert; break;
		case EDGE_DETECTION_PREWITT_HORIZONTAL: attrs->mask = hori; break;
		default: return false;
		}

		attrs->height = attrs->width = 3;
		attrs->type = type;
		return true;
	}
}
