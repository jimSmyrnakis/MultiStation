#include "EdgeDetectionMasks.h"

extern "C" {



	static float gx[2][2] = {
		{ 1, 0},
		{ 0,-1}
	};

	static float gy[2][2] = {
		{ 0, 1},
		{-1, 0}
	};

	bool robertsMask(enum MaskType type, struct MaskAttributes* attrs) {
		ASSERT(attrs, "Bad Parameter !!!");

		switch (type) {
		case EDGE_DETECTION_ROBERTS_GX: attrs->mask = gx; break;
		case EDGE_DETECTION_ROBERTS_GY: attrs->mask = gy; break;
		default: return false;
		}

		attrs->height = attrs->width = 2;
		attrs->type = type;
		return true;
	}
}