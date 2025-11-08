#ifndef ALLOCATOR_H
#define ALLOCATOR_H

	#include <stdint.h>
	#include <stddef.h>

	#ifdef __cplusplus
	extern "C" {
	#endif 
		typedef void* (*memory_allocate)(size_t size);
		typedef void  (*memory_free)(void* ptr);

		struct Allocator {
			memory_allocate memalloc;
			memory_free     memfree;
		};

	#ifdef __cplusplus
	}
	#endif 


#endif 
