#ifndef GAUSIAN_NOISE_H
#define GAUSIAN_NOISE_H

#include "Core.hpp"

#ifdef __cplusplus
extern "C" {
#endif 

	/**
	 * @author Dimitris Smyrnakis.
	 * @param[in] source , The Input BMP Image 
	 * @param[in] dest , The Output BMP Image
	 * @param[in] SNRdb , The Amplitute of the Gaussian distribution
	 * @returns true if the caculation and production of the new image was successfull , false if not .
	 * @details This function is a wrapper around the Pixel Buffer Object and ImageProcessing API that ask
	 * clearly the parameters and parsed them to the actual operation via the PBO API 
	 */
	bool image_GausianNoise(
		struct bmpImage* dest,
		const struct bmpImage* source,
		float SNRdb
	);

	/**
	 * @author.
	 */
	bool GausianNoiseOperation(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		struct Params* parms);

	bool GausianNoiseResolve(enum PBOType type, struct PBOAttributes* attrs);

#ifdef __cplusplus

}
#endif 

#endif 