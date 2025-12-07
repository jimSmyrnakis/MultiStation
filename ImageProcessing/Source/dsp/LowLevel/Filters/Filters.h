#pragma once
#include "../PixelBuffer.h"
extern "C" {

	/**
	* @author Dimitris Smyrnakis
	* @param dest Is the destination Pixel Buffer Object
	* @param src Is the source Pixel Buffer Object
	* @param parms Parameters related to the Pixel Buffer Object function
	* @details Caculates a new Pixel Buffer Object with median filter . 
	* parms this is where parameters parsed in to the function like the 
	* kernel size (NxN) that the operation of median filter needs and methods
	*  to allocate and the total number of parameters . This Function is a wrapper
	* around the PixelBufferApi for calling the Related MedianFilter Operation method
	*/
	bool MedianFilter(
		struct PixelBufferObject* dest,
		const struct PixelBufferObject* src,
		struct Params* parms
	);


	/**
	 * @author Dimitris Smyrnakis
	 * @param type Pixel Buffer Object Type 
	 * @param attrs Pixel Buffer Object Attributes (mainly Operation Method)
	 * @returns true if the type parsed is PBOT_MEDIAN_FILTER or false otherwise
	 * @details This method is a callback resolve method to the pixel buffer api 
	 * . If return true it says that yes the type you parse in ,
	 *  is the one i handle and here is the attributes (like the operation method to it)
	 */
	bool MedianFilterResolve(enum PBOType type, struct PBOAttributes* attrs);


	/**
	 * @author Dimitris Smyrnakis.
	 * @param dest destination bitmap Image format
	 * @param src source bitmap Image format
	 * @returns true if succeded and false otherwise
	 * @details A wrapper around the Pixel Buffer Api that calls the operation method of the
	 * median filter and parsed the right argument's , as well return's the result real image
	 * formated as needed . 
	 */
	bool ÉmageMedianFilter(
		struct bmpImage* dest,
		const struct bmpImage* source,
		uint32_t N , 
		Allocator allocator = {malloc , free});


}
