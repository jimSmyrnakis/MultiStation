#pragma once

#include <stdint.h>
#include <stddef.h>
#include "Error.hpp"
#include <Platform.hpp>


namespace MultiStation {
    /**
     * \author Dimitris Smyrnakis
	 * \details Initializes the graphics API with the specified viewport dimensions.
     * 
     */
	bool InitGraphicsApi(uint32_t ViewportWidth , uint32_t ViewportHeight);

}
