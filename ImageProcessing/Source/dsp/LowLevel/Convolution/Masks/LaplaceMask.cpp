#include "EdgeDetectionMasks.h"

extern "C" {



	static float negative[3][3] = {
		{ 0,-1, 0},
		{-1, 4,-1},
		{ 0,-1, 0}
	};

	static float positive[3][3] = {
		{ 0, 1, 0},
		{ 1,-4, 1},
		{ 0, 1, 0}
	};

	bool laplaceMask(enum MaskType type, struct MaskAttributes* attrs) {
		ASSERT(attrs, "Bad Parameter !!!");

		switch (type) {
		case EDGE_DETECTION_LAPLACE_NEGATIVE: attrs->mask = negative; break;
		case EDGE_DETECTION_LAPLACE_POSITIVE: attrs->mask = positive; break;
		default: return false;
		}

		attrs->height = attrs->width = 3;
		attrs->type = type;
		return true;
	}
}