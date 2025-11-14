#pragma once
#include "dsp/LowLevel/PointTransforms/Binarization.h"
#include "dsp/LowLevel/PointTransforms/GammaTransform.h"
#include "dsp/LowLevel/PointTransforms/Inverse.h"
#include "dsp/LowLevel/PointTransforms/Operations.h"
#include "dsp/LowLevel/PointTransforms/Rotation.h"
#include "dsp/LowLevel/PointTransforms/Sepia.h"
#include "dsp/LowLevel/PointTransforms/SaltAndPepperNoise.h"
#include "dsp/LowLevel/PointTransforms/GausianNoise.h"

#include "dsp/LowLevel/Converters.h"
#include "dsp/LowLevel/Extras.h"

#include "dsp/LowLevel/Histogram/Histogram.h"

#include "dsp/LowLevel/Convolution/Convolution.h"
#include "dsp/LowLevel/Convolution/Masks/Masks.h"



#include "Core.hpp"

int TestAll(void);
int TestLineDetector(void);
