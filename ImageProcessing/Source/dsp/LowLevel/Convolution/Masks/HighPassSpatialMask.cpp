#include "FilterMasks.h"

static float msk[3][3] = {
	{-1,-1,-1},
	{-1, 9,-1},
	{-1,-1,-1}
};

extern "C" bool highPassSpatialMask(enum MaskType type, struct MaskAttributes* attrs) {
	ASSERT(attrs, "Bad Parameter !");

	if (type != FILTER_HIGH_PASS_SPATIAL) return false;

	attrs->mask = msk;
	attrs->height = attrs->width = 3;
	attrs->type = type;

	return true;
}
